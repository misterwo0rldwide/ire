#include "ire/elf_program.h"

size_t elf_program_count(const ElfFile *elf) {
  if (elf == NULL) {
    return 0;
  }

  return ELF_EHDR_FIELD(elf->data, IS_64(elf), e_phnum);
}

const void *elf_program_header(const ElfFile *elf, size_t index) {
  size_t phdr_count;
  size_t phdr_entry_size;

  if (elf == NULL) {
    return NULL;
  }

  phdr_count = elf_program_count(elf);

  if (index >= phdr_count) {
    return NULL;
  }

  phdr_entry_size = ELF_EHDR_FIELD(elf->data, IS_64(elf), e_phentsize);
  return (const unsigned char *)elf->data +
         ELF_EHDR_FIELD(elf->data, IS_64(elf), e_phoff) +
         phdr_entry_size * index;
}

const void *elf_find_program(const ElfFile *elf, uint32_t type) {
  size_t phdr_count;
  const void *phdr_entry;

  if (elf == NULL) {
    return NULL;
  }

  phdr_count = elf_program_count(elf);

  for (size_t entry_index = 0; entry_index < phdr_count; entry_index++) {
    phdr_entry = elf_program_header(elf, entry_index);

    if (ELF_PHDR_FIELD(phdr_entry, IS_64(elf), p_type) == type) {
      return phdr_entry;
    }
  }

  return NULL;
}