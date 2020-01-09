#include "displayLegolas.h"

int main(int argc, char* argv[]) {
  FILE* file = fopen("Examples_loader/example1.o", "r");
  Header* header = legolasReadFromFile(file);

  printf("\n");
  printf("--------BEGINING TEST ON ELF HEADER----------\n");
  displayElfHeader(header);
  printf("----------END OF TEST ON ELF HEADER----------\n\n");

  printf("--------BEGINING TEST ON ELF SECTION HEADER TABLE---------\n");
  displayElfSectionHeaderTable(header);
  printf("----------END OF TEST ON ELF SECTION HEADER TABLE----------\n\n");

  printf("--------BEGINING TEST ON SECTION DATA EXTRACTING---------\n");
  displaySectionsRawData(header, ".strtab");
  printf("----------END OF TEST ON SECTION DATA EXTRACTING----------\n\n");

  printf("--------BEGINING TEST ON SYMBOLE TABLE EXTRACTING---------\n");
  displaySymbolTable(header);
  printf("----------END OF TEST ON SYMBOLE TABLE EXTRACTING----------\n\n");

  printf("--------BEGINING TEST ON REALOCATION TABLE EXTRACTING---------\n");
  displayRealocationTable(header);
  printf("----------END OF TEST ON REALOCATION TABLE EXTRACTING----------\n");

  fclose(file);
  return 0;
}
