#include "legolas.h"

char* getSymbolTableEntryName(Header* header, uint32_t indexName) {
    int indexStringTable = 0;

    /*
        Tant que le nom de la section qu'on analyse est différent de ".strtab" (i.e. le nom
        de la string table des symboles), on incrément notre indice.
        En sortie de while, indexStringTable = indice de la string table des symboles.
    */

    while(indexStringTable < header->shnum && (strcmp(header->sectionHeaderTable[indexStringTable]->name, ".strtab") != 0)) {
        indexStringTable++;
    }

    /*
        On vérifie qu'on soit bien en présence d'une string table pour symboles.
        Si ce n'est pas le cas, on quitte le programme avec une erreur.
    */

    if (strcmp(header->sectionHeaderTable[indexStringTable]->name, ".strtab") != 0) {
        printf("The string table for symbols was not found. Please make sure that your ELF file is not corrupted.");
        exit(1);
    }

    return ((header->sectionHeaderTable[indexStringTable]->data.stringTable) + indexName);
}

SymboleTableEntry* getSymboleTableEntryAddress(Header* header, uint32_t info) {
    unsigned int i = 0;

    /*
        Tant que le type de section est différent du type "symbol table", on incrémente i.
        En sortie de while, i = indice de la symbol table.
    */

    while (i < header->shnum && header->sectionHeaderTable[i]->type != SHT_SYMTAB) {
        i++;
    }

    /*
        On vérifie qu'on soit bien en présence d'une symbol table.
        Si ce n'est pas le cas, on quitte le programme avec une erreur.
    */

    if (header->sectionHeaderTable[i]->type != SHT_SYMTAB) {
        printf("The symbol table was not found. Please make sure that your ELF file is not corrupted.");
        exit(1);
    }

    return header->sectionHeaderTable[i]->data.symboleTable[info];
}

void symbolTableAddLocalEntry(SectionHeader* sectionHeader, SymboleTableEntry* symbolTableEntry, unsigned int index) {
   	//On vérifie que les section header et symbol table entry courantes ne soient pas NULL
   
   	if (sectionHeader == NULL) {
		printf("The current section header is NULL. The link edition will stop.");
		return;
    }
    if (symbolTableEntry == NULL) {
     	printf("The current symbol table entry is NULL. The link edition will stop.");
		return;
    }

	//Si la symbol table entry actuelle est de type locale, alors on la rajoute sans autre vérification
	
	if (symbolTableEntry->bind == STB_LOCAL) {
    	sectionHeader->data.symboleTable = realloc(sectionHeader->data.symboleTable, sizeof(SymboleTableEntry*) * (sectionHeader->nbEntry+1));
		sectionHeader->data.symboleTable[sectionHeader->nbEntry] = symbolTableEntry;
		sectionHeader->nbEntry++;
		sectionHeader->size += sizeof(SymboleTableEntry);
	}
}

void symbolTableAddGlobalEntry(SectionHeader* sectionHeader, SymboleTableEntry* symbolTableEntry, unsigned int index) {
     //On vérifie que les section header et symbol table entry courantes ne soient pas NULL.
	 
	 if (sectionHeader == NULL) {
     	 printf("The current section header is NULL. The link edition will stop.");
	 	 return;
     }
     if (symbolTableEntry == NULL) {
     	 printf("The current symbol table entry is NULL. The link edition will stop.");
	 	 return;
     }
	 
	 /*
	 	Si la symbol table entry actuelle est de type globale, alors on effectue diverses
		vérifications afin de voir si on la rajoute ou non.	
	 */

     if (symbolTableEntry->bind == STB_GLOBAL) {
		/*
			Booléen mis à jour si la symbol table entry courante matche
			avec une des symbol table entry du premier fichier.
		*/
		
		int noMatchInFirstSection = 0;

		//On boucle sur le nombre de symbol table entry du fichier 2.

        for (int i=0; i < sectionHeader->nbEntry; i++) {
            SymboleTableEntry* currentSymbolTableEntry = sectionHeader->data.symboleTable[i];

			/*
				On regarde si les symboles du fichier 1 et du fichier 2 courants ont le même nom,
				et on effectue d'autres vérifications le cas échéant.
			*/ 

            if (strcmp(symbolTableEntry->name, currentSymbolTableEntry->name) == 0) {
                //Si on passe ici, ça veut dire qu'on a un match
				
				noMatchInFirstSection = 1;
				
				//Si les 2 symboles courants sont définis, l'édition de lien échoue

				if (currentSymbolTableEntry->type != SHN_UNDEF && symbolTableEntry->type != SHN_UNDEF) {
                	printf("There are two global symbols defined with the same name if the 2 different files. The link edition will stop.");
		    		return;
            	}

				/*
					Si le symbole est défini dans le fichier 2 et indéfini dans le fichier 1,
					on le remplace par le symbole du fichier 2.
				*/

            	if (currentSymbolTableEntry->type == SHN_UNDEF && symbolTableEntry->type != SHN_UNDEF) {
                	symboleTableRemoveEntry(sectionHeader, symbolTableEntry);

					sectionHeader->data.symboleTable = realloc(sectionHeader->data.symboleTable, sizeof(SymboleTableEntry*) * (sectionHeader->nbEntry+1));
                	sectionHeader->data.symboleTable[sectionHeader->nbEntry] = symbolTableEntry;
                	sectionHeader->nbEntry++;
    		    	sectionHeader->size += sizeof(SymboleTableEntry);
            	}

				/*
					Si le symbole est défini dans le fichier 1 et indéfini dans le fichier 2,
					on ne l'ajoute pas, donc on ne fait rien.
				*/

            	else if (currentSymbolTableEntry->type != SHN_UNDEF && symbolTableEntry->type == SHN_UNDEF){}

				/*
					Si le symbole est indéfini dans le fichier 1 comme dans le fichier 2,
					on ne l'ajoute pas, donc on ne fait rien.
				*/

            	else if (currentSymbolTableEntry->type == SHN_UNDEF && symbolTableEntry->type == SHN_UNDEF){}
       		}
    	}

		/*
			Une fois qu'on a terminé la boucle, on regarde si le symbole actuel du fichier 2
			a matché avec un des symboles du fichier 1. 
			Si ce n'est pas le cas, on l'ajoute à la symbol table du fichier 1.
		*/

		if (noMatchInFirstSection == 0) {
         	sectionHeader->data.symboleTable = realloc(sectionHeader->data.symboleTable, sizeof(SymboleTableEntry*) * (sectionHeader->nbEntry+1));
         	sectionHeader->data.symboleTable[sectionHeader->nbEntry] = symbolTableEntry;
         	sectionHeader->nbEntry++;	
	     	sectionHeader->size += sizeof(SymboleTableEntry);
		}
    }
}

int symbolTableGetEntryIndex(SectionHeader* sectionHeader, SymboleTableEntry* symboleTableEntry) {
    int i = 0;

	//On cherche l'index de la symbol table entry actuelle dans la symbol table	

    while (i < sectionHeader->nbEntry && sectionHeader->data.symboleTable[i] != symboleTableEntry) {
        i++;
    }

	/*
		Si l'index retourné est supérieur au nombre total de symbol table entry,
		c'est que la symbol table entry n'a pas été trouvée, donc on renvoie une
		erreur.
	*/

    if (i >= sectionHeader->nbEntry) {
        printf("Error : symbol table entry %p not found in the symbol table\n", symboleTableEntry);
        i = -1;
    }

    return i;
}

void symboleTableRemoveEntry(SectionHeader* sectionHeader, SymboleTableEntry* symboleTableEntry) {
	/*
		On supprime la symbol table entry actuelle de la symbol table
		en remplaçant la symbol table entry à supprimer par le dernier et en
		réduisant le nombre de symbol table entry de 1.
	*/
		
	int i = symbolTableGetEntryIndex(sectionHeader, symboleTableEntry);
    sectionHeader->data.symboleTable[i] = sectionHeader->data.symboleTable[sectionHeader->nbEntry-1];
    sectionHeader->nbEntry--;
	sectionHeader->size -= sizeof(SymboleTableEntry);
    //free(symboleTableEntry);
}

Elf32_Sym* sectionHeaderGetSymbolData(Header* header, SectionHeader* sectionHeader, SectionHeader* stringTable) {
	//On malloc la symbol table de la taille d'une entrée * le nombre d'entrées
	
	Elf32_Sym* symbolTable = malloc(sizeof(Elf32_Sym) * sectionHeader->nbEntry);
	
	//On rajoute toutes les informations des entrées dans notre symbol table

	for (int i=0; i < sectionHeader->nbEntry; i++) {
		Elf32_Sym* currentNewSymbolTableEntry = &(symbolTable[i]);
		SymboleTableEntry* currentOldSymbolTableEntry = sectionHeader->data.symboleTable[i];

		currentNewSymbolTableEntry->st_value = currentOldSymbolTableEntry->value;
		currentNewSymbolTableEntry->st_size = reverseEndian32(currentOldSymbolTableEntry->size);
		currentNewSymbolTableEntry->st_other = currentOldSymbolTableEntry->other;
		currentNewSymbolTableEntry->st_name = reverseEndian32(stringTableGetIndex(stringTable, currentOldSymbolTableEntry->name));
		currentNewSymbolTableEntry->st_info = ELF32_ST_INFO(currentOldSymbolTableEntry->bind, currentOldSymbolTableEntry->type);
		currentNewSymbolTableEntry->st_shndx = reverseEndian16(headerGetIndexOfSectionHeader(header, currentOldSymbolTableEntry->sectionHeader));
	}

	return symbolTable;
}

void changeSymbolTableEntryPointerOnSectionHeaderOnFusion(Header* header, SectionHeader* sectionHeaderFile2, SectionHeader* sectionHeaderFile1) {
     /*
	 	Après fusion des sections PROGBITS, on doit faire pointer les symbol table entry
		vers les nouvelles sections créées par la fusion (actuellement, elles pointent sur
		des sections qui "n'existent plus").
	 */

	 for (int k=0; k < header->shnum; k++) {
          SectionHeader* symboleTableHeader = header->sectionHeaderTable[k];

          if (symboleTableHeader->type == SHT_SYMTAB) {
               for (int l=0; l < symboleTableHeader->nbEntry; l++) {
                    if (symboleTableHeader->data.symboleTable[l]->sectionHeader == sectionHeaderFile2) {
                         symboleTableHeader->data.symboleTable[l]->sectionHeader = sectionHeaderFile1;
                    }
               }
               break;
          }
     }
}
