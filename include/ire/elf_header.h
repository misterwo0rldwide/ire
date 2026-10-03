#ifndef IRE_ELF_HEADER_H
#define IRE_ELF_HEADER_H

#include "ire/elf_file.h"

#define ELF_EHDR_FIELD(data, is_64, field)                                     \
  ((is_64) ? ((const Elf64_Ehdr *)(data))->field                               \
           : ((const Elf32_Ehdr *)(data))->field)

unsigned char get_endianness(ElfFile *);
unsigned char get_class(ElfFile *);

#endif