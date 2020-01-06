#include "etape6.h"

int main(int argc, char* argv[]) {

  FILE* file1 = fopen(argv[1], "r");
  FILE* file2 = fopen(argv[2], "r");
  FILE* file3 = fopen(argv[3], "w");

  fusionProgbit(file1, file2, file3);

  return 0;
}
