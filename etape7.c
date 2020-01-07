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
			  unsigned int nbGlobalSymbolInFirstFile = 0;
			  SymboleTableEntry** globalSymbolTable = malloc(sizeof(SymboleTableEntry*));
			  
			  for (int k=0; k < sh1->nbEntry; k++) {
			  	  if (sh1->data.symboleTable[k]->bind == STB_GLOBAL) {
				  	  nbGlobalSymbolInFirstFile++;
					  globalSymbolTable = realloc(globalSymbolTable, sizeof(SymboleTableEntry*) * nbGlobalSymbolInFirstFile);
					  globalSymbolTable[k] = sh1->data.symboleTable[k];

					  symboleTableRemoveEntry(sh1, sh1->data.symboleTable[k]);
				  }
			  }

              for (int k=0; k < sh2->nbEntry; k++) {
                  symbolTableAddLocalEntry(sh1, sh2->data.symboleTable[k], k);	
			  }

			  for (int k=0; k < nbGlobalSymbolInFirstFile; k++) {
			  	  symbolTableAddGlobalEntry(sh1, globalSymbolTable[k], k);
			  }

			  for (int k=0; k < sh2->nbEntry; k++) {
			      symbolTableAddGlobalEntry(sh1, sh2->data.symboleTable[k], k);
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

  return 0;
}
