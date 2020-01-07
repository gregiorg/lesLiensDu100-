#include "etape6.h"
#include <errno.h>


int main(int argc, char* argv[]) {

  FILE* file1 = fopen(argv[1], "r");
  FILE* file2 = fopen(argv[2], "r");
  FILE* file3 = fopen(argv[3], "w");

  if (file1 == NULL || file2 == NULL || file3 == NULL) {
		fprintf(stderr, "Value of errno: %d\n", errno);
		fprintf(stderr, "Error opening the file: %s\n", strerror( errno ));
		perror("Error printed by perror");
		exit(EXIT_FAILURE);
	}

  ElfHeader* elfHeaderEndian = malloc(sizeof(ElfHeader));
  freadElfHEader(elfHeaderEndian, sizeof(*elfHeaderEndian), 1, file1);

  programsEndian = elfHeaderEndian->indentData;

  fusionProgbit(file1, file2, file3);

  fclose(file1);
  fclose(file2);
  fclose(file3);

  return 0;
}
