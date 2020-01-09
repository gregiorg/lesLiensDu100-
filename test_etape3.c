#include "etape3.h"

extern int errno;

int main(int argc, char* argv[]) {

  FILE* file = fopenR(argv[1], "r");

  ElfHeader* elfHeaderEndian = malloc(sizeof(ElfHeader));
  freadElfHEader(elfHeaderEndian, sizeof(*elfHeaderEndian), 1, file);

  programsEndian = elfHeaderEndian->indentData;

  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(file);
  uint32_t* data = readDataNumSec(file, 1);
  afficherDataSection(data, elfSecHeader, 1);

  fclose(file);

  return 0;
}
