//------------------------AFFICHAGE DU CONTENU D'UNE SECTION-------------------
#include "etape3.h"

//A partir du numéro de section
uint32_t* readDataNumSec(FILE* file, int numSection) {
  long int filePos = ftell(file);
  ElfHeaderF* elfHeader = getElfHeader(file);
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(file);

  uint32_t* data = NULL;

  if(numSection < elfHeader->shnum && numSection >= 0) {
    fseek(file, elfSecHeader[numSection]->offset, SEEK_SET);

    data = malloc(elfSecHeader[numSection]->size);
    freadData(data, elfSecHeader[numSection]->size, 1, file);
  }
  fseek(file, filePos, SEEK_SET);
  return data;
}

//A partir du nom de section
uint32_t* readDataNomSec(FILE* file, char* nomSection) {
  long int filePos = ftell(file);
  ElfHeaderF* elfHeader = getElfHeader(file);
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(file);

  int i = 0;
  while(i < elfHeader->shnum && strcmp(elfSecHeader[i]->nameStr, nomSection)!=0) {
    i++;
  }

  uint32_t* data = NULL;

  if(i < elfHeader->shnum) {
    fseek(file, elfSecHeader[i]->offset, SEEK_SET);

    data = malloc(elfSecHeader[i]->size);
    freadData(data, elfSecHeader[i]->size, 1, file);
  }
  fseek(file, filePos, SEEK_SET);
  return data;
}

//Affichage des données
void afficherDataSection(uint32_t* data, ElfSecHeaderF** elfSecHeader, int numSection) {
  for (int i = 0; i < elfSecHeader[numSection]->size/sizeof(uint32_t); i++) {
      printf("%08X\n", reverseEndian32(data[i]));
    }
}
