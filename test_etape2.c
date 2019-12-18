#include "etape2.h"

int main(int argc, char* argv[]) {

  FILE* f = fopen(argv[1], "r");

  ElfHeaderF* elfHeader = getElfHeader(f);
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(f);

  afficherTabSecHeader(elfSecHeader, elfHeader->shnum);

  fclose(f);

  return 0;
}
