#include "ire/checksec.h"
#include "ire/elf_file.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
  ElfFile elf;
  ChecksecResult result;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <elf-file>\n", argv[0]);
    return 1;
  }

  if (elf_open(argv[1], &elf) != 0) {
    fprintf(stderr, "Failed to open ELF file: %s\n", argv[1]);
    return 1;
  }

  if (elf_checksec(&elf, &result) != 0) {
    fprintf(stderr, "Failed to analyze ELF file\n");
    elf_close(&elf);
    return 1;
  }

  printf("File:   %s\n", argv[1]);
  printf("NX:     %s\n", result.nx ? "Enabled" : "Disabled");
  printf("PIE:    %s\n", result.pie ? "Enabled" : "Disabled");
  printf("RELRO:  %s\n", result.relro == 2   ? "Full"
                         : result.relro == 1 ? "Partial"
                                             : "None");
  printf("Canary: %s\n", result.canary ? "Found" : "Not found");

  elf_close(&elf);
  return 0;
}