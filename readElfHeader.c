#include "etape1.h"

extern int programsEndian;

int main(int argc, char* argv[]) {
  FILE* file = fopenR(argv[1], "r");

  ElfHeader* elfHeaderEndian = malloc(sizeof(ElfHeader));
  freadElfHEader(elfHeaderEndian, sizeof(*elfHeaderEndian), 1, file);

  programsEndian = elfHeaderEndian->indentData;

  ElfHeaderF* elfHeader = getElfHeader(file);


  afficherHeader(elfHeader);

  fclose(file);

  return 0;
}
