#include <stdio.h>
#include <stdint.h>

#define ELF32_ST_BIND(i)   ((i)>>4)
#define ELF32_ST_TYPE(i)   ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

typedef struct {
  uint32_t    st_name;   // Elf32_Word
  uint32_t    st_value;  // Elf32_Addr
  uint32_t    st_size;   // Elf32_Word
  unsigned char st_info;
  unsigned char st_other;
  uint16_t    st_shndx;  // Elf32_Half
} Elf32_Sym;

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
