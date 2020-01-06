#include <stdio.h>
#include <string.h>
#include "legolas.h"
#include "etape4.h"

int main(int argc, char* argv[]) {
  FILE* firstFile = fopen(argv[1], "r");
  FILE* secondFile = fopen(argv[2], "r");

  /* A FAIRE QUAND LEGOLAS SERA FINI
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

  // extracting the first elf header, the symbol table section header and the number of symboles
  ElfHeaderF* firstElfHeader = getElfHeader(firstFile);
  ElfSecHeaderF** firstElfSecHeader = getTabElfSecHeader(firstFile);
  int iterator = 0;
  while(iterator < firstElfHeader->shnum && strcmp(firstElfSecHeader[iterator]->nameStr, ".symtab")!=0) {
    iterator++;
  }
  int firstNbElm = (firstElfSecHeader[iterator]->size) / (firstElfSecHeader[iterator]->entSize);

  // extracting the second elf header, the symbol table section header and the number of symboles
  ElfHeaderF* secondElfHeader = getElfHeader(secondFile);
  ElfSecHeaderF** secondElfSecHeader = getTabElfSecHeader(secondFile);
  iterator = 0;
  while(iterator < secondElfHeader->shnum && strcmp(secondElfSecHeader[iterator]->nameStr, ".symtab")!=0) {
    iterator++;
  }
  int secondNbElm = (secondElfSecHeader[iterator]->size) / (secondElfSecHeader[iterator]->entSize);

  //extracting the symbole table
  Elf32Sym** firstSymbolTable = getTabSym(firstFile);
  Elf32Sym** secondSymbolTable = getTabSym(secondFile);

  // allocationg space for the final symbol table
  // we allocate more than necessary
  Elf32Sym** finalSymbolTable = malloc((firstNbElm + secondNbElm) * sizeof(Elf32Sym*));
  for(int i = 0; i < firstNbElm + secondNbElm; i++) {
    finalSymbolTable[i] = malloc(sizeof(Elf32Sym));
  }

  // current index in in the symbole table
  int currentIndex = 0;

  // extracting the local symbols in the first file
  for(int i = 0; i < firstNbElm; i++) {
    if (ELF32_ST_BIND(firstSymbolTable[i]->stInfo) == STB_LOCAL) {
      finalSymbolTable[currentIndex] = firstSymbolTable[i];
      currentIndex++;
    }
  }

  // extracting the local symbols in the second file
  for(int i = 0; i < secondNbElm; i++) {
    if (ELF32_ST_BIND(secondSymbolTable[i]->stInfo) == STB_LOCAL) {
      finalSymbolTable[currentIndex] = secondSymbolTable[i];
      currentIndex++;
    }
  }



  return 0;
}
