#include "etape2.h"
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

  ElfHeaderF* elfHeader = getElfHeader(file);
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(file);

  afficherTabSecHeader(elfSecHeader, elfHeader->shnum);

  fclose(file);

  return 0;
}
