#include "etape3.h"

void afficheTabSym(Elf32_Sym* tab_sym, int size) {
  for (int i = 0; i < size; i++) {
    printf("symbole n°%d\n", i);
    printf("nom du symbole : %d\n", tab_sym[i].st_name);
    printf("valeur du symbole : %d\n", tab_sym[i].st_value);
    printf("taille du symbole : %d\n", tab_sym[i].st_size);
    printf("\n");
  }
}

int main(int argc, char *argv[]) {

  Elf_SecHeaderF* elf_sec_header = etape2(argv[1]);
  FILE* file = fopen(argv[1], "r");
  ElfHeaderF* elf_header = get_elf_header(file);

  int i = 0;
  while(i < elf_header->shnum && !strcmp(elf_sec_header[i].nameStr, ".symtab")) {
    i++;
  }

  uint32_t* data = read_data_nomsec(argv[1], ".symtab");
  Elf32_Sym* tab_sym = malloc(elf_sec_header[i].size);

  for (int i = 0; i < elf_sec_header[i].size; i+=4) {
    tab_sym[i/4].st_name = data[i];
    tab_sym[i/4].st_value = data[i+1];
    tab_sym[i/4].st_size = data[i+2];

    uint32_t sauv = data[i+3];
    tab_sym[i/4].st_info = sauv >> 24;
    tab_sym[i/4].st_other = (sauv >> 16) & 0x00FF;
    tab_sym[i/4].st_shndx = sauv & 0xFFFF;
  }

  afficheTabSym(tab_sym, elf_sec_header[i].size);

  return 0;
}
