#ifndef IRE_ELF_SECTION_H
#define IRE_ELF_SECTION_H

#include "ire/elf_header.h"

#define ELF_SHDR_FIELD(data, is_64, field)                                     \
  ((is_64) ? ((const Elf64_Shdr *)(data))->field                               \
           : ((const Elf32_Shdr *)(data))->field)

size_t elf_section_count(const ElfFile *);
const void *elf_section_header(const ElfFile *, size_t);
const void *elf_find_section(const ElfFile *, uint32_t);

const char *elf_section_name(const ElfFile *, const void *);
const void *elf_find_section_by_name(const ElfFile *, const char *);

#endif