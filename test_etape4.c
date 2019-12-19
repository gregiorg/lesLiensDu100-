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

  int a = 0;
  while(a < elfHeader->shnum && strcmp(elfSecHeader[a]->nameStr, ".strtab")!=0) {
    a++;
  }
  uint32_t stringTableAddress = getAddressStringTable(elfHeader->shoff, elfHeader->shentsize, elfSecHeader[a]->offset, f);

  Elf32Sym** tabSym = getTabSym(f);
  afficherTabSym(tabSym, nbElm, stringTableAddress, f);

  fclose(f);

  return 0;
}
