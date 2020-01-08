#include "etape6.h"
#include <errno.h>


int main(int argc, char* argv[]) {

  FILE* file1 = fopen(argv[1], "r");
  FILE* file2 = fopen(argv[2], "r");

  if (file1 == NULL || file2 == NULL) {
		fprintf(stderr, "Value of errno: %d\n", errno);
		fprintf(stderr, "Error opening the file: %s\n", strerror( errno ));
		perror("Error printed by perror");
		exit(EXIT_FAILURE);
	}

  fusionProgbit(file1, file2, argv[3]);

  fclose(file1);
  fclose(file2);

  return 0;
}
