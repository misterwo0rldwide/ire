#ifndef IRE_ELF_FILE_H
#define IRE_ELF_FILE_H

#include <elf.h>
#include <stddef.h>

#define FILE_START_OFFSET (0)
#define IS_64(elf) elf->class == ELFCLASS64

typedef struct {
  int fd;
  size_t size;
  void *data;

  unsigned char class;
} ElfFile;

int elf_open(const char *path, ElfFile *elf);
void elf_close(ElfFile *elf);

#endif