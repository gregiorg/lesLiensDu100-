#include "displayLegolas.h"

int main(int argc, char* argv[]) {
  FILE* file = fopen("Examples_loader/example1.o", "r");
  Header* header = legolasReadFromFile(file);

  printf("--------BEGINING TEST ON ELF HEADER----------\n");
  displayElfHeader(header);
  printf("----------END OF TEST ON ELF HEADER----------\n\n");

  printf("--------BEGINING TEST ON ELF SECTION HEADER TABLE---------\n");
  displayElfSectionHeaderTable(header);
  printf("----------END OF TEST ON ELF SECTION HEADER TABLE----------\n\n");

  fclose(file);
  return 0;
}
