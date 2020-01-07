#include "etape4.h"
#include <errno.h>

extern int errno;


int main(int argc, char* argv[]) {

  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    fprintf(stderr, "Value of errno: %d\n", errno);
    fprintf(stderr, "Error opening the file: %s\n", strerror( errno ));
    perror("Error printed by perror");
    exit(EXIT_FAILURE);
  }

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
