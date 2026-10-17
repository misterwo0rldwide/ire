#ifndef IRE_CHECKSEC_H
#define IRE_CHECKSEC_H

#include "ire/elf_file.h"
#include <stdbool.h>

typedef struct {
  bool pie;
  bool nx;
  bool canary;

  /* 0 = none, 1 = partial, 2 = full */
  int relro;
} ChecksecResult;

int elf_checksec(const ElfFile *, ChecksecResult *);

#endif