#include <stdio.h>
#include <string.h>
#include "legolas.h"

int main(int argc, char* argv[]) {

  FILE* file1 = fopen(argv[1], "r");
  FILE* file2 = fopen(argv[2], "r");
  FILE* file3 = fopen(argv[3], "w");

  Header* h1 = legolasReadFromFile(file1);
  Header* h2 = legolasReadFromFile(file2);

  // integration des sections du header 2 dans le header 1
  for (int i = 0; i < h2->shnum; i++) {
      SectionHeader* sh2 = h2->sectionHeaderTable[i];

      int fusion = 0;

      // fusion des sections fusionnables
      for (int j = 0; j < h1->shnum; j++) {
          SectionHeader* sh1 = h1->sectionHeaderTable[j];

          // sections de type progbit
          if (sh2->type == SHT_PROGBITS && sh1->type == SHT_PROGBITS && strcmp(sh2->name, sh1->name)==0) {
              fusion = 1;

              sh1->rawData = realloc(sh1->rawData, sh1->size + sh2->size);
              memcpy(((char*) sh1->rawData) + sh1->size, sh2->rawData, sh2->size);
              sh1->size += sh2->size;

              break;
          }

          // sections de type table des symboles
          if (sh2->type == SHT_SYMTAB && sh1->type == SHT_SYMTAB) {
              fusion = 1;

              for (int k=0; k < sh2->nbEntry; k++) {
                  symbolTableAddEntry(sh1, sh2->data.symboleTable[k]);
              }

              break;
          }
      }

      // sinon ajout de la section
      if (fusion == 0) {
          headerAddSection(h1, sh2);
      }
  }

  legolasWriteToFile(h1, file3);

  /* A FAIR QUAND LEGOLAS EST FINI
  Header* firstHeader = legolasReadFromFile(firstFile);
  Header* secondHeader = legolasReadFromFile(secondFile);

  int firstIndexOfSymbolTable = 0;
  while(firstIndexOfSymbolTable < firstHeader->shnum && strcmp(firstHeader->sectionHeaderTable[firstIndexOfSymbolTable]->name, ".symtab") != 0) {
    firstIndexOfSymbolTable++;
  }

  int secondIndexOfSymbolTable = 0;
  while(secondIndexOfSymbolTable < secondHeader->shnum && strcmp(secondHeader->sectionHeaderTable[secondIndexOfSymbolTable]->name, ".symtab") != 0) {
    secondIndexOfSymbolTable++;
  }

  SectionHeader* firstSymbolTable = firstHeader->sectionHeaderTable[firstIndexOfSymbolTable];
  SectionHeader* secondSymbolTable = secondHeader->sectionHeaderTable[secondIndexOfSymbolTable];

  printf("%s\n", firstSymbolTable->data.symboleTable[0]->name);
  printf("%s\n", secondSymbolTable->data.symboleTable[0]->name);
  */
/*
  ElfHeaderF* firstElfHeader = getElfHeader(firstFile);
  ElfSecHeaderF** firstElfSecHeader = getTabElfSecHeader(firstFile);
  int iterator = 0;
  while(iterator < firstElfHeader->shnum && strcmp(firstElfSecHeader[iterator]->nameStr, ".symtab")!=0) {
    iterator++;
  }
  int firstNbElm = (firstElfSecHeader[iterator]->size) / (firstElfSecHeader[iterator]->entSize);

  ElfHeaderF* secondElfHeader = getElfHeader(secondFile);
  ElfSecHeaderF** secondElfSecHeader = getTabElfSecHeader(secondFile);
  iterator = 0;
  while(iterator < secondElfHeader->shnum && strcmp(secondElfSecHeader[iterator]->nameStr, ".symtab")!=0) {
    iterator++;
  }
  int secondNbElm = (secondElfSecHeader[iterator]->size) / (secondElfSecHeader[iterator]->entSize);

  Elf32Sym** firstSymbolTable = getTabSym(firstFile);
  Elf32Sym** secondSymbolTable = getTabSym(secondFile);

  Elf32Sym** finalSymbolTable = malloc((firstNbElm + secondNbElm) * sizeof(Elf32Sym*));
  for(int i = 0; i < firstNbElm + secondNbElm; i++) {
    finalSymbolTable[i] = malloc(sizeof(Elf32Sym));
  }

  // int currentIndex = 0;

  for(int i = 0; i < firstNbElm; i++) {
    printf("%d\n", ELF32_ST_BIND(firstSymbolTable[i]->stInfo));
  }

  for(int i = 0; i < secondNbElm; i++) {
    printf("%d\n", ELF32_ST_BIND(secondSymbolTable[i]->stInfo));
  }

  return 0;
*/
  return 0;
}
