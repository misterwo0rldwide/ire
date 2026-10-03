#include "ire/elf_header.h"

unsigned char get_endianness(ElfFile *elf) {
  if (elf == NULL) {
    return ELFDATANONE;
  }
  return ((unsigned char *)elf->data)[EI_DATA];
}

unsigned char get_class(ElfFile *elf) {
  if (elf == NULL) {
    return ELFCLASSNONE;
  }
  return ((unsigned char *)elf->data)[EI_CLASS];
}