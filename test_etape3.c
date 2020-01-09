#include "etape3.h"

extern int errno;

int main(int argc, char* argv[]) {

  FILE* file = fopenR(argv[1], "r");
  if (file == NULL) {
		fprintf(stderr, "Value of errno: %d\n", errno);
		fprintf(stderr, "Error opening the file: %s\n", strerror( errno ));
		perror("Error printed by perror");
		exit(EXIT_FAILURE);
	}

  ElfHeader* elfHeaderEndian = malloc(sizeof(ElfHeader));
  freadElfHEader(elfHeaderEndian, sizeof(*elfHeaderEndian), 1, file);

  programsEndian = elfHeaderEndian->indentData;

  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(file);
  uint32_t* data = readDataNumSec(file, 1);
  afficherDataSection(data, elfSecHeader, 1);

  fclose(file);

  return 0;
}
