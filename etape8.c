#include "etape8.h"

void fusionRelocationTable(FILE* file1, FILE* file2, FILE* fileR){

  Header* h1 = legolasReadFromFile(file1);
  Header* h2 = legolasReadFromFile(file2);

  // fusion des sections progbits et des sections de types inconnues
  for (int i = 0; i < h2->shnum; i++) {
      SectionHeader* sh2 = h2->sectionHeaderTable[i];

      int fusion = 0;

      for (int j = 0; j < h1->shnum; j++) {
          SectionHeader* sh1 = h1->sectionHeaderTable[j];

          // sections de type progbit
          if (sh2->type == SHT_PROGBITS && sh1->type == SHT_PROGBITS && strcmp(sh2->name, sh1->name)==0) {
              fusion = 1;

              sh1->rawData = realloc(sh1->rawData, sh1->size + sh2->size);
              memcpy(((char*) sh1->rawData) + sh1->size, sh2->rawData, sh2->size);
              sh1->size += sh2->size;

              changeSymbolTableEntryPointerOnSectionHeaderOnFusion(h2, sh2, sh1);
              updateRelocationTableEntryOffsetOnSectionHeaderOnFusion(h2, sh2, sh1);

              break;
          }

          // sections de type table des symboles
          if (sh2->type == SHT_SYMTAB) {
              fusion = 1;
              break;
          }

          // sections de type table des relocation
          if (sh2->type == SHT_REL || sh2->type == SHT_RELA) {
              fusion = 1;
              break;
          }
      }

      // sinon ajout de la section
      if (fusion == 0) {
          headerAddSection(h1, sh2);
      }
  }

  // fusion des sections table de symboles
  for (int i=0; i < h2->shnum; i++) {
      SectionHeader* sh2 = h2->sectionHeaderTable[i];
	  int matchName = 0;

      for (int j = 0; j < h1->shnum; j++) {
          SectionHeader* sh1 = h1->sectionHeaderTable[j];

          if (sh2->type == SHT_SYMTAB && sh1->type == SHT_SYMTAB) {
			  unsigned int nbGlobalSymbolInFirstFile = 0;
			  SymboleTableEntry** globalSymbolTable = malloc(sizeof(SymboleTableEntry*));

			  sh1->info = 0;

			  for (int k=0; k < sh1->nbEntry; k++) {
			  	  if (sh1->data.symboleTable[k]->bind == STB_GLOBAL) {
				  	  nbGlobalSymbolInFirstFile++;
					  globalSymbolTable = realloc(globalSymbolTable, sizeof(SymboleTableEntry*) * nbGlobalSymbolInFirstFile);
					  globalSymbolTable[k] = sh1->data.symboleTable[k];

					  symboleTableRemoveEntry(sh1, sh1->data.symboleTable[k]);
				  }
				  else {
				  	  sh1->info++;
				  }
			  }

              for (int k=0; k < sh2->nbEntry; k++) {
				  sh1->info++;
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

          if (sh2->type == SHT_REL && sh1->type == SHT_REL && strcmp(sh1->name, sh2->name) == 0) {
              matchName = 1;

			  for (int k = 0; k < sh2->nbEntry; k++) {
                  relocationTableAddEntry(sh1, sh2->data.relocationTable[k]);
              }

              break;
          }
	  }

	  if (matchName == 0 && sh2->type == SHT_REL) {
	  	  printf("Warning : The relocation table %p in the second file has no matching table in the first file\n", sh2);
	  	  headerAddSection(h1, sh2);
	  }
  }

  legolasWriteToFile(h1, fileR);
}
