#include <stdio.h>
#include <string.h>
#include "legolas.h"
#include "etape4.h"

int main(int argc, char* argv[]) {
  FILE* firstFile = fopen(argv[1], "r");
  FILE* secondFile = fopen(argv[2], "r");

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
}
