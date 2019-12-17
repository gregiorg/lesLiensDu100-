#include "etape3.h"

uint32_t* read_data(char* fname, int nsection) {
  FILE* file = fopen(fname, "r");
  ElfHeader elf_header;
  fread(&elf_header, sizeof(elf_header), 1, file);

  fseek(file, reverse_endian_32(elf_header.shoff) + nsection*reverse_endian_16(elf_header.shentsize), SEEK_SET);

  Elf_SecHeader elf_sec_header;
  fread(&elf_sec_header, sizeof(elf_sec_header), 1, file);

  fseek(file, reverse_endian_32(elf_sec_header.offset), SEEK_SET);

  uint32_t* data = malloc(reverse_endian_32(elf_sec_header.size));
  fread(data, reverse_endian_32(elf_sec_header.size), 1, file);

  for (int i = 0; i < reverse_endian_32(elf_sec_header.size)/sizeof(uint32_t); i++)
      printf("%08X\n", reverse_endian_32(data[i]));

  return data;
}

uint32_t* read_data(char* fname, char* nsection) {
  Elf_SecHeaderF elf_sec_header = etape2(fname);
  
}
