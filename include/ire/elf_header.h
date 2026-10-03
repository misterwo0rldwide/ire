#ifndef IRE_ELF_HEADER_H
#define IRE_ELF_HEADER_H

#include "ire/elf_file.h"

unsigned char get_endianness(ElfFile *);
unsigned char get_class(ElfFile *);

#endif