#ifndef IRE_ELF_PROGRAM_H
#define IRE_ELF_PROGRAM_H

#include "ire/elf_header.h"

#define ELF_PHDR_FIELD(data, is_64, field)                                     \
  ((is_64) ? ((const Elf64_Phdr *)(data))->field                               \
           : ((const Elf32_Phdr *)(data))->field)

size_t elf_program_count(const ElfFile *elf);
const void *elf_program_header(const ElfFile *elf, size_t index);
const void *elf_find_program(const ElfFile *elf, uint32_t type);

#endif