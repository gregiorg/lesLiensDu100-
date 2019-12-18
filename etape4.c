#include "etape4.h"

int main(int argc, char* argv[]) {
  FILE* file;
  if((file = fopen(argv[1], "r"))) {
    etape4(file);
    fclose(file);
  } else {
    printf("C'est de la merde!!!!!\n");
  }
  return 0;
}

void afficheTabSym(Elf32Sym** tabSym, int size, uint32_t stringTableAddress, FILE* f) {
  for (int i = 0; i < size; i++) {
    printf("Symbole n°%d :\n", i);
    printf("	Indice du nom du symbole : %08X\n", tabSym[i]->stName);
    printf("	Nom du symbole : %s\n", showName(tabSym[i]->stName, stringTableAddress, f));
    printf("	Valeur du symbole : %08X\n", tabSym[i]->stValue);
    printf("	Taille du symbole : %08X\n", tabSym[i]->stSize);
    printf("	NDX du symbole : %d\n", tabSym[i]->stShndx);
    printf("\n");
  }
}

Elf32Sym** etape4(FILE* f) {

  ElfHeaderF* elfHeader = getElfHeader(f);
  ElfSecHeaderF** elfSecHeader = etape2(f);

  uint32_t stringTableAddress = getAddressStringTable(elfHeader->shentsize, elfHeader->shstrndx, f);

  int i = 0;
  while(i < elfHeader->shnum && strcmp(elfSecHeader[i]->nameStr, ".symtab")!=0) {
    i++;
  }

  uint32_t* data = readDataNomSec(f, ".symtab");

  int nbElm = (elfSecHeader[i]->size) / (elfSecHeader[i]->entSize);

  Elf32Sym** tabSym = malloc(nbElm * sizeof(Elf32Sym*));

  for(int k = 0; k < nbElm; k++) {
    tabSym[k] = malloc(sizeof(Elf32Sym));
  }

  for (int j = 0; j < nbElm; j++) {
    tabSym[j]->stName = reverseEndian32(data[j*4]);
    tabSym[j]->stValue = reverseEndian32(data[j*4+1]);
    tabSym[j]->stSize = reverseEndian32(data[j*4+2]);

    uint32_t sauv = reverseEndian32(data[j*4+3]);
    tabSym[j]->stInfo = sauv >> 24;
    tabSym[j]->stOther = (sauv >> 16) & 0x00FF;
    tabSym[j]->stShndx = sauv & 0xFFFF;
  }
  afficheTabSym(tabSym, nbElm, stringTableAddress, f);

  return tabSym;
}
