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

  int indexOfStrTab = 0;
  while(indexOfStrTab < elfHeader->shnum && strcmp(elfSecHeader[indexOfStrTab]->nameStr, ".strtab")!=0) {
    indexOfStrTab++;
  }
  printf("appel qui nous interese\n");
  uint32_t stringTableAddress = getAddressStringTable(elfHeader->shoff, elfHeader->shentsize, indexOfStrTab, f);

  Elf32Sym** tabSym = getTabSym(f);
  afficherTabSym(tabSym, nbElm, stringTableAddress, f);

  fclose(f);

  return 0;
}
