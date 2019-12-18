#include "etape1.h"

int main(int argc, char* argv[]) {
  FILE* file = fopen(argv[1], "r");

  ElfHeaderF* elfHeader = getElfHeader(file);
  afficherHeader(elfHeader);

  fclose(file);

  return 0;
}
