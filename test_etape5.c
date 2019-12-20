#include "etape5.h"
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
