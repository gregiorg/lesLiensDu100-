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

  ElfHeaderF* elfHeader = getElfHeader(file);
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(file);
  int i = 0;
  while(i < elfHeader->shnum && strcmp(elfSecHeader[i]->nameStr, ".symtab")!=0) {
    i++;
  }
  int nbElm = (elfSecHeader[i]->size) / (elfSecHeader[i]->entSize);

  int a = 0;
  while(a < elfHeader->shnum && strcmp(elfSecHeader[a]->nameStr, ".strtab")!=0) {
    a++;
  }
  uint32_t stringTableAddress = getAddressStringTable(elfHeader->shoff, elfHeader->shentsize, elfSecHeader[a]->offset, file);

  Elf32Sym** tabSym = getTabSym(file);
  afficherTabSym(tabSym, nbElm, stringTableAddress, file);

  fclose(file);

  return 0;
}
