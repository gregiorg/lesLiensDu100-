#include "etape3.h"

uint32_t* read_data_numsec(char* fname, int nsection) {
  Elf_SecHeaderF* elf_sec_header = etape2(fname);
  FILE* file = fopen(fname, "r");
  ElfHeaderF* elf_header = get_elf_header(file);

  uint32_t* data = NULL;

  if(nsection < elf_header->shnum && nsection >= 0) {
    fseek(file, elf_sec_header[nsection].offset, SEEK_SET);

    data = malloc(elf_sec_header[nsection].size);
    fread(data, elf_sec_header[nsection].size, 1, file);

    for (int i = 0; i < elf_sec_header[nsection].size/sizeof(uint32_t); i++)
        printf("%08X\n", reverse_endian_32(data[i]));
  }

  return data;
}

uint32_t* read_data_nomsec(char* fname, char* nsection) {
  Elf_SecHeaderF* elf_sec_header = etape2(fname);
  FILE* file = fopen(fname, "r");
  ElfHeaderF* elf_header = get_elf_header(file);

  int i = 0;
  while(i < elf_header->shnum && !strcmp(elf_sec_header[i].nameStr, nsection)) {
    i++;
  }

  uint32_t* data = NULL;

  if(i < elf_header->shnum) {
    fseek(file, elf_sec_header[i].offset, SEEK_SET);

    data = malloc(elf_sec_header[i].size);
    fread(data, elf_sec_header[i].size, 1, file);

    for (int j = 0; j < elf_sec_header[i].size/sizeof(uint32_t); j++)
        printf("%08X\n", reverse_endian_32(data[j]));
  }

  return data;
}
