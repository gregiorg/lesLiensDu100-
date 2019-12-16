#include <stdio.h>
#include <stdint.h>
#include "util.h"

int main(int argc, char const *argv[]) {

  FILE* file = fopen(argv[1], "r");

  Elf32_Sym elf_sym;
  while(!feof(file)) {
    fread(&elf_sym, sizeof (Elf32_Sym), 1, file);

    printf("%d\n", elf_sym.st_name);
    printf("%d\n", elf_sym.st_value);

    printf("%d\n", ELF32_ST_TYPE(elf_sym.st_info));
    printf("%d\n", ELF32_ST_BIND(elf_sym.st_info));
  }

  return 0;
}
