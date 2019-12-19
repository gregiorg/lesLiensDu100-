#include "etape5.h"

int main(int argc, char* argv[]) {

  FILE* file = fopen(argv[1], "r");

  RealocationEntryF** realocTable = getRealocationTable(file); // TODO : find the infinite loop

  ElfHeaderF* header = getElfHeader(file);
  ElfSecHeaderF** sectionTable = getTabElfSecHeader(file);

  int nbrRel = 0;
  for (int i = 0; i < header->shnum; i++) {
    ElfSecHeaderF* sectionHeader = sectionTable[i];
    if (sectionHeader->typeInt == SH_REL) {
      nbrRel++;
    }
  }

  afficherRealocationTab(realocTable, nbrRel);

  return 0;
}
