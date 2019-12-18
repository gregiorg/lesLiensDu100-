#include "etape3.h"

uint32_t* readDataNumSec(FILE* f, int numSection) {
  ElfHeaderF* elfHeader = getElfHeader(f);
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(f);

  uint32_t* data = NULL;

  if(numSection < elfHeader->shnum && numSection >= 0) {
    fseek(f, elfSecHeader[numSection]->offset, SEEK_SET);

    data = malloc(elfSecHeader[numSection]->size);
    fread(data, elfSecHeader[numSection]->size, 1, f);
  }
  return data;
}

uint32_t* readDataNomSec(FILE* f, char* nomSection) {
  ElfHeaderF* elfHeader = getElfHeader(f);
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(f);

  int i = 0;
  while(i < elfHeader->shnum && strcmp(elfSecHeader[i]->nameStr, nomSection)!=0) {
    i++;
  }

  uint32_t* data = NULL;

  if(i < elfHeader->shnum) {
    fseek(f, elfSecHeader[i]->offset, SEEK_SET);

    data = malloc(elfSecHeader[i]->size);
    fread(data, elfSecHeader[i]->size, 1, f);
  }
  return data;
}

void afficherDataSection(uint32_t* data, ElfSecHeaderF** elfSecHeader, int numSection) {
  for (int i = 0; i < elfSecHeader[numSection]->size/sizeof(uint32_t); i++) {
      printf("%08X\n", reverseEndian32(data[i]));
    }
}
