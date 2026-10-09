
#include <string.h>

#include "ire/elf_section.h"
#include "ire/elf_symbol.h"

int elf_symbol_table(const ElfFile *elf, ElfSymbolTable *table, uint32_t type) {
  size_t strtab_index;
  if (elf == NULL || table == NULL ||
      (type != SHT_SYMTAB && type != SHT_DYNSYM)) {
    return 0;
  }

  table->elf = elf;
  table->symtab = elf_find_section(elf, type);
  table->strtab = NULL;

  if (table->symtab == NULL) {
    return 0;
  }

  strtab_index = ELF_SHDR_FIELD(table->symtab, IS_64(elf), sh_link);
  table->strtab = elf_section_header(elf, strtab_index);
  return table->strtab != NULL &&
         ELF_SHDR_FIELD(table->strtab, IS_64(elf), sh_type) == SHT_STRTAB;
}

size_t elf_symbol_count(const ElfSymbolTable *table) {
  size_t entry_size;
  if (table == NULL || table->elf == NULL || table->symtab == NULL) {
    return 0;
  }

  entry_size = ELF_SHDR_FIELD(table->symtab, IS_64(table->elf), sh_entsize);
  if (entry_size == 0) {
    return 0;
  }

  return ELF_SHDR_FIELD(table->symtab, IS_64(table->elf), sh_size) / entry_size;
}

const void *elf_symbol(const ElfSymbolTable *table, size_t index) {
  size_t entry_size;
  size_t count;
  size_t offset;
  if (table == NULL || table->elf == NULL || table->symtab == NULL) {
    return NULL;
  }

  count = elf_symbol_count(table);
  if (index >= count) {
    return NULL;
  }

  entry_size = ELF_SHDR_FIELD(table->symtab, IS_64(table->elf), sh_entsize);
  offset = ELF_SHDR_FIELD(table->symtab, IS_64(table->elf), sh_offset);
  return (const unsigned char *)table->elf->data + offset + index * entry_size;
}

const char *elf_symbol_name(const ElfSymbolTable *table, const void *symbol) {
  size_t name_offset;
  size_t strtab_offset;
  size_t strtab_size;
  const char *name;
  const char *end;
  if (table == NULL || table->elf == NULL || table->strtab == NULL ||
      symbol == NULL) {
    return NULL;
  }

  name_offset = ELF_SYMBOL_FIELD(symbol, IS_64(table->elf), st_name);
  strtab_offset = ELF_SHDR_FIELD(table->strtab, IS_64(table->elf), sh_offset);
  strtab_size = ELF_SHDR_FIELD(table->strtab, IS_64(table->elf), sh_size);

  if (name_offset >= strtab_size || strtab_offset > table->elf->size ||
      strtab_size > table->elf->size - strtab_offset) {
    return NULL;
  }

  name = (const char *)table->elf->data + strtab_offset + name_offset;
  end = memchr(name, '\0', strtab_size - name_offset);
  return end != NULL ? name : NULL;
}