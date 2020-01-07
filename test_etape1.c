#include "etape1.h"
#include <errno.h>

extern int errno;

extern int programsEndian;

int main(int argc, char* argv[]) {
  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
		fprintf(stderr, "Value of errno: %d\n", errno);
		fprintf(stderr, "Error opening the file: %s\n", strerror( errno ));
		perror("Error printed by perror");
		exit(EXIT_FAILURE);
	}

  ElfHeader* elfHeaderEndian = malloc(sizeof(ElfHeader));
  freadElfHEader(elfHeaderEndian, sizeof(*elfHeaderEndian), 1, file);

  programsEndian = elfHeaderEndian->indentData;

  ElfHeaderF* elfHeader = getElfHeader(file);


  afficherHeader(elfHeader);

  fclose(file);

  return 0;
}
