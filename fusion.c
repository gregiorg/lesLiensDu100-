#include "etape8.h"

int main(int argc, const char* argv[]) {
    const char* fName1 = argv[1];
    const char* fName2 = argv[2];
    const char* fNameR = argv[3];

    FILE* file1 = fopenR(fName1, "r");
    FILE* file2 = fopenR(fName2, "r");
    FILE* fileR = fopenR(fNameR, "w");

    fusionRelocationTable(file1, file2, fileR);

    fclose(file1);
    fclose(file2);
    fclose(fileR);

    return 0;
}
