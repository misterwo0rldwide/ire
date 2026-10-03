#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "ire/elf_file.h"

int elf_open(const char *path, ElfFile *elf) {
  struct stat file_status;
  if (NULL == elf) {
    return -1;
  }

  elf->fd = open(path, O_RDONLY);
  if (elf->fd == -1) {
    return -1;
  }

  if (fstat(elf->fd, &file_status) < 0) {
    close(elf->fd);
    return -1;
  }

  elf->size = file_status.st_size;
  elf->data =
      mmap(NULL, elf->size, PROT_READ, MAP_PRIVATE, elf->fd, FILE_START_OFFSET);
  if (elf->data == MAP_FAILED) {
    close(elf->fd);
    return -1;
  }

  elf->class = ((unsigned char *)elf->data)[EI_CLASS];
  return 0;
}

void elf_close(ElfFile *elf) {
  if (NULL == elf) {
    return;
  }

  munmap(elf->data, elf->size);
  close(elf->fd);
}