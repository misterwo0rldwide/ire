
#include "ire/elf_dynamic.h"
#include "ire/elf_program.h"

#include <elf.h>

static const void *elf_dynamic_segment(const ElfFile *elf) {
  const void *phdr;
  size_t offset;
  size_t size;

  if (elf == NULL || elf->data == NULL) {
    return NULL;
  }

  phdr = elf_find_program(elf, PT_DYNAMIC);
  if (phdr == NULL) {
    return NULL;
  }

  offset = ELF_PHDR_FIELD(phdr, IS_64(elf), p_offset);
  size = ELF_PHDR_FIELD(phdr, IS_64(elf), p_filesz);

  if (offset > elf->size || size > elf->size - offset) {
    return NULL;
  }

  return (const unsigned char *)elf->data + offset;
}

size_t elf_dynamic_count(const ElfFile *elf) {
  const void *phdr;
  size_t size;
  size_t entry_size;

  if (elf == NULL || elf->data == NULL) {
    return 0;
  }

  phdr = elf_find_program(elf, PT_DYNAMIC);
  if (phdr == NULL) {
    return 0;
  }

  size = ELF_PHDR_FIELD(phdr, IS_64(elf), p_filesz);
  entry_size = IS_64(elf) ? sizeof(Elf64_Dyn) : sizeof(Elf32_Dyn);

  return size / entry_size;
}

const void *elf_dynamic_entry(const ElfFile *elf, size_t index) {
  const unsigned char *segment;
  size_t count;
  size_t entry_size;

  count = elf_dynamic_count(elf);
  if (index >= count) {
    return NULL;
  }

  segment = elf_dynamic_segment(elf);
  if (segment == NULL) {
    return NULL;
  }

  entry_size = IS_64(elf) ? sizeof(Elf64_Dyn) : sizeof(Elf32_Dyn);

  return segment + index * entry_size;
}

const void *elf_find_dynamic(const ElfFile *elf, int64_t tag) {
  const void *entry;
  size_t count;

  count = elf_dynamic_count(elf);

  for (size_t i = 0; i < count; i++) {
    entry = elf_dynamic_entry(elf, i);
    if (entry == NULL) {
      return NULL;
    }

    if (ELF_DYN_FIELD(entry, IS_64(elf), d_tag) == tag) {
      return entry;
    }

    if (ELF_DYN_FIELD(entry, IS_64(elf), d_tag) == DT_NULL) {
      break;
    }
  }

  return NULL;
}
