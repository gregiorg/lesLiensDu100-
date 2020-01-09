#include "etape5.h"

extern int errno;

int main(int argc, char* argv[]) {

  FILE* file = fopenR(argv[1], "r");

  ElfHeader* elfHeaderEndian = malloc(sizeof(ElfHeader));
  freadElfHEader(elfHeaderEndian, sizeof(*elfHeaderEndian), 1, file);

  programsEndian = elfHeaderEndian->indentData;

  ElfHeaderF* header = getElfHeader(file);
  ElfSecHeaderF** sectionTable = getTabElfSecHeader(file);

  int nbrRelEnt = 0;
  for (int i = 0; i < header->shnum; i++) {
    if (sectionTable[i]->typeInt == SHT_REL) {
      nbrRelEnt += sectionTable[i]->size / sectionTable[i]->entSize;
    }
  }

  RealocationEntryF** realocTable = getRealocationTable(file);

  afficherRealocationTab(realocTable, nbrRelEnt);

  fclose(file);

  return 0;
}
