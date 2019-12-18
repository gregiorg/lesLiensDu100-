#include "etape3.h"

int main(int argc, char* argv[]) {

  FILE* f = fopen(argv[1], "r");

  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(f);
  uint32_t* data = readDataNumSec(f, 1);
  afficherDataSection(data, elfSecHeader, 1);

  fclose(f);

  return 0;
}
