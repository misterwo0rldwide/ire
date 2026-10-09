
#ifndef IRE_ELF_SYMBOL_H
#define IRE_ELF_SYMBOL_H

#include "ire/elf_header.h"

#define ELF_SYMBOL_FIELD(data, is_64, field)                                   \
  ((is_64) ? ((const Elf64_Sym *)(data))->field                                \
           : ((const Elf32_Sym *)(data))->field)

typedef struct {
  const ElfFile *elf;
  const void *symtab;
  const void *strtab;
} ElfSymbolTable;

int elf_symbol_table(const ElfFile *, ElfSymbolTable *, uint32_t type);

size_t elf_symbol_count(const ElfSymbolTable *);
const void *elf_symbol(const ElfSymbolTable *, size_t index);
const char *elf_symbol_name(const ElfSymbolTable *, const void *);

#endif