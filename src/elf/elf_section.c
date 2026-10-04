#include <string.h>

#include "ire/elf_section.h"

size_t elf_section_count(const ElfFile *elf) {
  if (elf == NULL) {
    return 0;
  }

  return ELF_EHDR_FIELD(elf->data, IS_64(elf), e_shnum);
}

const void *elf_section_header(const ElfFile *elf, size_t index) {
  size_t shdr_count;
  size_t shdr_entry_size;

  if (elf == NULL) {
    return NULL;
  }

  shdr_count = elf_section_count(elf);

  if (index >= shdr_count) {
    return NULL;
  }

  shdr_entry_size = ELF_EHDR_FIELD(elf->data, IS_64(elf), e_shentsize);
  return (const unsigned char *)elf->data +
         ELF_EHDR_FIELD(elf->data, IS_64(elf), e_shoff) +
         shdr_entry_size * index;
}

const void *elf_find_section(const ElfFile *elf, uint32_t type) {
  size_t shdr_count;
  const void *shdr_entry;

  if (elf == NULL) {
    return NULL;
  }

  shdr_count = elf_section_count(elf);

  for (size_t entry_index = 0; entry_index < shdr_count; entry_index++) {
    shdr_entry = elf_section_header(elf, entry_index);

    if (ELF_SHDR_FIELD(shdr_entry, IS_64(elf), sh_type) == type) {
      return shdr_entry;
    }
  }

  return NULL;
}

const char *elf_section_name(const ElfFile *elf, const void *section) {
  uint16_t shstrndx;
  const void *shstrtab;

  if (elf == NULL || section == NULL) {
    return NULL;
  }

  shstrndx = ELF_EHDR_FIELD(elf, IS_64(elf), e_shstrndx);
  shstrtab = elf_section_header(elf, shstrndx);

  if (shstrtab == NULL) {
    return NULL;
  }

  return (const char *)elf->data +
         ELF_SHDR_FIELD(shstrtab, IS_64(elf), sh_offset) +
         ELF_SHDR_FIELD(section, IS_64(elf), sh_name);
}

const void *elf_find_section_by_name(const ElfFile *elf, const char *name) {
  size_t shdr_count;
  const void *shdr_entry;
  const char *section_name;

  if (elf == NULL || name == NULL) {
    return NULL;
  }

  shdr_count = elf_section_count(elf);

  for (size_t entry_index = 0; entry_index < shdr_count; entry_index++) {
    shdr_entry = elf_section_header(elf, entry_index);
    section_name = elf_section_name(elf, shdr_entry);

    if (!strcmp(section_name, name)) {
      return shdr_entry;
    }
  }

  return NULL;
}