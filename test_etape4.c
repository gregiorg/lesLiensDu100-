#include "etape4.h"

int main(int argc, char* argv[]) {

  FILE* f = fopen(argv[1], "r");

  ElfHeaderF* elfHeader = getElfHeader(f);
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(f);
  int i = 0;
  while(i < elfHeader->shnum && strcmp(elfSecHeader[i]->nameStr, ".symtab")!=0) {
    i++;
  }
  int nbElm = (elfSecHeader[i]->size) / (elfSecHeader[i]->entSize);
  uint32_t stringTableAddress = getAddressStringTable(elfHeader->shentsize, elfHeader->shstrndx, f);
  printf("debug : 0x%x\n", stringTableAddress);

  Elf32Sym** tabSym = getTabSym(f);
  afficherTabSym(tabSym, nbElm, stringTableAddress, f);

  fclose(f);

  return 0;
}
