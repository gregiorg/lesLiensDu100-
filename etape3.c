#include "etape3.h"

uint32_t* readDataNumSec(FILE* file, int numSection) {
  ElfHeaderF* elfHeader = getElfHeader(file);
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(file);

  uint32_t* data = NULL;

  if(numSection < elfHeader->shnum && numSection >= 0) {
    fseek(file, elfSecHeader[numSection]->offset, SEEK_SET);

    data = malloc(elfSecHeader[numSection]->size);
    size_t codeRet = fread(data, elfSecHeader[numSection]->size, 1, file);
    if(codeRet != 1) {
        if (feof(file)){
          printf("Erreur de lecture du fichier: fin de fichier inattendue\n");
          exit(EXIT_FAILURE);
        } else if (ferror(file)) {
          perror("Erreur de lecture du fichier");
          exit(EXIT_FAILURE);
        }
    }
  }
  return data;
}

uint32_t* readDataNomSec(FILE* file, char* nomSection) {
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
    size_t codeRet = fread(data, elfSecHeader[i]->size, 1, file);
    if(codeRet != 1) {
        if (feof(file)){
          printf("Erreur de lecture du fichier: fin de fichier inattendue\n");
          exit(EXIT_FAILURE);
        } else if (ferror(file)) {
          perror("Erreur de lecture du fichier");
          exit(EXIT_FAILURE);
        }
    }
  }
  return data;
}

void afficherDataSection(uint32_t* data, ElfSecHeaderF** elfSecHeader, int numSection) {
  for (int i = 0; i < elfSecHeader[numSection]->size/sizeof(uint32_t); i++) {
      printf("%08X\n", reverseEndian32(data[i]));
    }
}
