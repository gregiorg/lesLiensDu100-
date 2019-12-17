#include "etape3.h"

void afficheTabSym(Elf32Sym** tabSym, int size) {
  for (int i = 0; i < size; i++) {
    printf("symbole n°%d\n", i);
    printf("nom du symbole : %d\n", tabSym[i]->stName);
    printf("valeur du symbole : %d\n", tabSym[i]->stValue);
    printf("taille du symbole : %d\n", tabSym[i]->stSize);
    printf("\n");
  }
}

int etape4(FILE* f) {

  ElfHeaderF* elfHeader = getElfHeader(f);
  ElfSecHeaderF** elfSecHeader = etape2(f);

  int i = 0;
  while(i < elfHeader->shnum && !strcmp(elfSecHeader[i]->nameStr, ".symtab")) {
    i++;
  }

  uint32_t* data = read_data_nomsec(argv[1], ".symtab");
  Elf32Sym** tabSym = malloc(elfSecHeader[i]->size);

  for (int i = 0; i < elfSecHeader[i]->size; i+=4) {
    tabSym[i/4]->stName = data[i];
    tabSym[i/4]->stValue = data[i+1];
    tabSym[i/4]->stSize = data[i+2];

    uint32_t sauv = data[i+3];
    tabSym[i/4]->stInfo = sauv >> 24;
    tabSym[i/4]->stOther = (sauv >> 16) & 0x00FF;
    tabSym[i/4]->stShndx = sauv & 0xFFFF;
  }

  afficheTabSym(tabSym, elfSecHeader[i].size);

  return 0;
}
