#include "etape2.h"

int main(int argc, char* argv[]) {

  FILE* file = fopenR(argv[1], "r");

  ElfHeader* elfHeaderEndian = malloc(sizeof(ElfHeader));
  freadElfHEader(elfHeaderEndian, sizeof(*elfHeaderEndian), 1, file);

  programsEndian = elfHeaderEndian->indentData;

  ElfHeaderF* elfHeader = getElfHeader(file);
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(file);

  afficherTabSecHeader(elfSecHeader, elfHeader->shnum);

  fclose(file);

  return 0;
}
