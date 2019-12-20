#include "etape5.h"

int main(int argc, char* argv[]) {

  FILE* file = fopen(argv[1], "r");

  ElfHeaderF* header = getElfHeader(file);
  ElfSecHeaderF** sectionTable = getTabElfSecHeader(file);

  int nbrRelEnt = 0;
  for (int i = 0; i < header->shnum; i++) {
    if (sectionTable[i]->typeInt == SH_REL) {
      nbrRelEnt += sectionTable[i]->size / sectionTable[i]->entSize;
    }
  }

  RealocationEntryF** realocTable = getRealocationTable(file);

  afficherRealocationTab(realocTable, nbrRelEnt);

  fclose(file);

  return 0;
}
