#ifndef IRE_ELF_DYNAMIC_H
#define IRE_ELF_DYNAMIC_H

#include "ire/elf_header.h"

#include <stdint.h>

#define ELF_DYN_FIELD(data, is_64, field)                                      \
  ((is_64) ? ((const Elf64_Dyn *)(data))->field                                \
           : ((const Elf32_Dyn *)(data))->field)

size_t elf_dynamic_count(const ElfFile *elf);
const void *elf_dynamic_entry(const ElfFile *elf, size_t index);
const void *elf_find_dynamic(const ElfFile *elf, int64_t tag);

#endif
