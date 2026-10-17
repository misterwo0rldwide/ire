#include "ire/checksec.h"
#include "ire/elf_dynamic.h"
#include "ire/elf_header.h"
#include "ire/elf_program.h"
#include "ire/elf_section.h"
#include "ire/elf_symbol.h"

#include <string.h>

int elf_checksec_relro(const ElfFile *elf) {
  const void *entry;
  Elf64_Xword flags;

  if (elf == NULL || elf_find_program(elf, PT_GNU_RELRO) == NULL) {
    return 0;
  }

  if (elf_find_dynamic(elf, DT_BIND_NOW) != NULL) {
    return 2;
  }

  entry = elf_find_dynamic(elf, DT_FLAGS);
  if (entry != NULL) {
    flags = ELF_DYN_FIELD(entry, IS_64(elf), d_un.d_val);
    if (flags & DF_BIND_NOW) {
      return 2;
    }
  }

  entry = elf_find_dynamic(elf, DT_FLAGS_1);
  if (entry != NULL) {
    flags = ELF_DYN_FIELD(entry, IS_64(elf), d_un.d_val);
    if (flags & DF_1_NOW) {
      return 2;
    }
  }

  return 1;
}

int elf_checksec_canary(const ElfFile *elf) {
  ElfSymbolTable table;

  if (!elf_symbol_table(elf, &table, SHT_DYNSYM)) {
    return 0;
  }

  for (size_t i = 0; i < elf_symbol_count(&table); i++) {
    const void *symbol = elf_symbol(&table, i);
    const char *name = elf_symbol_name(&table, symbol);

    if (name != NULL && strcmp(name, "__stack_chk_fail") == 0) {
      return 1;
    }
  }

  return 0;
}

int elf_checksec(const ElfFile *elf, ChecksecResult *result) {
  void *data;
  if (elf == NULL || result == NULL) {
    return -1;
  }

  data = (void *)elf_find_program(elf, PT_GNU_STACK);
  result->nx =
      data != NULL && !(ELF_PHDR_FIELD(data, IS_64(elf), p_flags) & PF_X);
  result->pie = ELF_EHDR_FIELD(elf, IS_64(elf), e_type) == ET_EXEC;
  result->relro = elf_checksec_relro(elf);
  result->canary = elf_checksec_canary(elf);
  return 0;
}