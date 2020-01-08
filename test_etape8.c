#include "etape8.h"

int main(int argc, const char* argv[]) {
    const char* fName1 = argv[1];
    const char* fName2 = argv[2];
    const char* fName3 = argv[3];

    FILE* file1 = fopen(fName1, "r");
    FILE* file2 = fopen(fName2, "r");

    fusionRelocationTable(file1, file2, fName3);

    fclose(file1);
    fclose(file2);

    return 0;
}
