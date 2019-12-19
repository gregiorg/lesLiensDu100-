//------------------------AFFICHAGE DU CONTENU D'UNE SECTION-------------------
#include "etape3.h"

//A partir du numéro de section
uint32_t* readDataNumSec(FILE* f, int numSection) {
  long int filePos = ftell(f);
  ElfHeaderF* elfHeader = getElfHeader(f);
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(f);

  uint32_t* data = NULL;

  if(numSection < elfHeader->shnum && numSection >= 0) {
    fseek(f, elfSecHeader[numSection]->offset, SEEK_SET);

    data = malloc(elfSecHeader[numSection]->size);
    size_t codeRet = fread(data, elfSecHeader[numSection]->size, 1, f);
    if(codeRet != 1) {
        if (feof(f)){
          printf("Erreur de lecture du fichier: fin de fichier inattendue\n");
          exit(EXIT_FAILURE);
        } else if (ferror(f)) {
          perror("Erreur de lecture du fichier");
          exit(EXIT_FAILURE);
        }
    }
  }
  fseek(f, filePos, SEEK_SET);
  return data;
}

//A partir du nom de section
uint32_t* readDataNomSec(FILE* f, char* nomSection) {
  long int filePos = ftell(f);
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
    size_t codeRet = fread(data, elfSecHeader[i]->size, 1, f);
    if(codeRet != 1) {
        if (feof(f)){
          printf("Erreur de lecture du fichier: fin de fichier inattendue\n");
          exit(EXIT_FAILURE);
        } else if (ferror(f)) {
          perror("Erreur de lecture du fichier");
          exit(EXIT_FAILURE);
        }
    }
  }
  fseek(f, filePos, SEEK_SET);
  return data;
}

//Affichage des données
void afficherDataSection(uint32_t* data, ElfSecHeaderF** elfSecHeader, int numSection) {
  for (int i = 0; i < elfSecHeader[numSection]->size/sizeof(uint32_t); i++) {
      printf("%08X\n", reverseEndian32(data[i]));
    }
}
