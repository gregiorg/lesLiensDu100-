#include "etape6.h"

int main(int argc, char* argv[]) {

  FILE* file1 = fopenR(argv[1], "r");
  FILE* file2 = fopenR(argv[2], "r");
  
  fusionProgbit(file1, file2, argv[3]);

  fclose(file1);
  fclose(file2);

  return 0;
}
