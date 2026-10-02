#ifndef IRE_ELF_FILE_H
#define IRE_ELF_FILE_H

#include <stddef.h>
#include <elf.h>

#define FILE_START_OFFSET (0)

typedef struct {
    int fd;
    size_t size;
    void *data;

    Elf64_Ehdr *header;
} ElfFile;

int elf_open(const char *path, ElfFile *elf);
void elf_close(ElfFile *elf);

#endif