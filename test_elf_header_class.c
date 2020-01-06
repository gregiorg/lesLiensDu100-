#include "elf_header_class.h"

int main(int argc, char** argv) {
	FILE* file1 = fopen(argv[1], "r");
//	FILE* file2 = fopen(argv[2], "w");

	if (file1 == NULL ) { // || file2 == NULL) {
		return 1;
	}

<<<<<<< HEAD
// Header* header = headerFromFile(file1);
=======
	//Header* header = headerFromFile(file1);
>>>>>>> bd95f79f5c8cb3d28a84030d64af1a01229accae
//	headerWriteToFile(header, file2);

	fclose(file1);
//	fclose(file2);

	return 0;
}
