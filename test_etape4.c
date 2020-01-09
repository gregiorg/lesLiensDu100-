#include "etape4.h"

extern int errno;


int main(int argc, char* argv[]) {

  FILE* file = fopenR(argv[1], "r");

  ElfHeader* elfHeaderEndian = malloc(sizeof(ElfHeader));
  freadElfHEader(elfHeaderEndian, sizeof(*elfHeaderEndian), 1, file);

  programsEndian = elfHeaderEndian->indentData;

  ElfHeaderF* elfHeader = getElfHeader(file);
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(file);
  int i = 0;
  while(i < elfHeader->shnum && strcmp(elfSecHeader[i]->nameStr, ".symtab")!=0) {
    i++;
  }
  int nbElm = (elfSecHeader[i]->size) / (elfSecHeader[i]->entSize);

  int indexOfStrTab = 0;
  while(indexOfStrTab < elfHeader->shnum && strcmp(elfSecHeader[indexOfStrTab]->nameStr, ".strtab")!=0) {
    indexOfStrTab++;
  }

  uint32_t stringTableAddress = getAddressStringTable(elfHeader->shoff, elfHeader->shentsize, indexOfStrTab, file);

  Elf32Sym** tabSym = getTabSym(file);
  afficherTabSym(tabSym, nbElm, stringTableAddress, file);

  fclose(file);

  return 0;
}
