#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "legolas.h"

void setHeader(Elf32_Ehdr* fileHeader, Header* header) {
    header->indentClass = fileHeader->e_ident[EI_CLASS];
    header->indentData  = fileHeader->e_ident[EI_DATA];
	programsEndian = fileHeader->e_ident[EI_DATA];
    header->indentVersion = fileHeader->e_ident[EI_VERSION];
    header->indentOSABI = fileHeader->e_ident[EI_OSABI];
    header->indentABIVersion = fileHeader->e_ident[EI_ABIVERSION];
    header->type = reverseEndian16(fileHeader->e_type);
    header->machine = reverseEndian16(fileHeader->e_machine);
    header->version = reverseEndian32(fileHeader->e_version);
    header->entry = reverseEndian32(fileHeader->e_entry);
    header->flags = reverseEndian32(fileHeader->e_flags);
    header->shnum = reverseEndian16(fileHeader->e_shnum);
}

void setSectionHeader(Header* header, Elf32_Shdr* fileSectionHeaderTable, int i, FILE* file) {
	header->sectionHeaderTable[i] = malloc(sizeof(SectionHeader));

	SectionHeader* sectionHeader = header->sectionHeaderTable[i];
	Elf32_Shdr* fileSectionHeader = &(fileSectionHeaderTable[i]);

	sectionHeader->type = reverseEndian32(fileSectionHeader->sh_type);
	sectionHeader->flags = reverseEndian32(fileSectionHeader->sh_flags);
	sectionHeader->addr = reverseEndian32(fileSectionHeader->sh_addr);
	sectionHeader->link = reverseEndian32(fileSectionHeader->sh_link);
	sectionHeader->info = reverseEndian32(fileSectionHeader->sh_info);
	sectionHeader->addrAlign = reverseEndian32(fileSectionHeader->sh_addralign);
	sectionHeader->size = reverseEndian32(fileSectionHeader->sh_size);
    sectionHeader->entSize = reverseEndian32(fileSectionHeader->sh_entsize);
	sectionHeader->rawData = malloc(sectionHeader->size);

    if (sectionHeader->entSize == 0) {
        sectionHeader->nbEntry = 0;
    } else {
        sectionHeader->nbEntry = sectionHeader->size / sectionHeader->entSize;
    }

	fseek(file, reverseEndian32(fileSectionHeader->sh_offset), SEEK_SET);
    fread(sectionHeader->rawData, sectionHeader->size, 1, file);

    typeFirstRawDataPartIfNeeded(sectionHeader, header);
}

void typeFirstRawDataPartIfNeeded(SectionHeader* currentSectionHeader, Header* header) {
    //On switch sur le type de section pour savoir si on doit ou non la traiter

    switch(currentSectionHeader->type) {
        case SHT_STRTAB: {
            /*               
                Dans le cas d'une string table, on fait juste pointer la stringTable de la
                section courante sur la rawData castée en (char*).
            */

            currentSectionHeader->data.stringTable = (char*) currentSectionHeader->rawData;
            break;
        }

        case SHT_REL: {
            //On compte le nombre d'entrées de la table de relocation courante

            uint32_t nbRelocationTableEntry = (currentSectionHeader->size / currentSectionHeader->entSize);

            //On malloc notre tableau de pointeurs sur relocation entry 

            currentSectionHeader->data.relocationTable = malloc (sizeof(void*) * nbRelocationTableEntry);

            //Pour chaque relocation entry, on remplit le plus d'informations possibles

            for (int i=0; i < nbRelocationTableEntry; i++) {
                //On malloc notre relocation entry courante de la taille d'une RelocationTableEntry

                RelocationTableEntry* currentRelocationTableEntry = malloc (sizeof(RelocationTableEntry));

                //On fait pointer une RealocationEntry sur le début de la rawData concernée

                RealocationEntry* relocationEntry = (RealocationEntry*) &(((char*) currentSectionHeader->rawData)[currentSectionHeader->entSize * i]);

                //On ajoute le type de relocation entry a notre relocation entry courante

                currentRelocationTableEntry->type = ELF32_R_TYPE(reverseEndian32(relocationEntry->info));

                //On remplit la i-ème case de notre tableau avec la relocation entry courante

                currentSectionHeader->data.relocationTable[i] = currentRelocationTableEntry;
            }

            break;
        }

        case SHT_SYMTAB: {
            //On compte le nombre d'entrées de la table de symboles courante
            uint32_t nbSymbolTableEntry = (currentSectionHeader->size / currentSectionHeader->entSize);
            
            //On malloc notre tableau de pointeurs sur symbol entry

            currentSectionHeader->data.symboleTable = malloc (sizeof(void*) * nbSymbolTableEntry);

            //Pour chaque symbol entry, on remplit le plus d'informations possibles

            for (int i=0; i < nbSymbolTableEntry; i++) {
                //On malloc notre symbol entry courante de la taille d'une SymbolTableEntry

                SymboleTableEntry* currentSymbolTableEntry = malloc (sizeof(SymboleTableEntry));

                //On fait pointer une Elf32Sym sur le début de la rawData concernée

                Elf32Sym* elf32Sym = (Elf32Sym*) &(((char*) currentSectionHeader->rawData)[currentSectionHeader->entSize * i]);

                //On ajoute toutes les informations possibles à notre symbol entry courante

                currentSymbolTableEntry->value = elf32Sym->stValue;
                currentSymbolTableEntry->size = elf32Sym->stSize;
                currentSymbolTableEntry->other = elf32Sym->stOther;
                currentSymbolTableEntry->bind = ELF32_ST_BIND(elf32Sym->stInfo);
                currentSymbolTableEntry->type = ELF32_ST_TYPE(elf32Sym->stInfo);

                //On remplit la i-ème case de notre tableau avec la symbol entry courante

                currentSectionHeader->data.symboleTable[i] = currentSymbolTableEntry;
            }

            break;
        }
    }
}

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

SectionHeader* getSectionHeaderAddress(Header* header, uint16_t shndx) {
    /*
        Si l'indice transmis est supérieur au nombre de sections total,
        on quitte le programme avec une erreur.
    */

    if (shndx > header->shnum - 1) {
        printf("L'indice transmis est supérieur au nombre de sections total");
        exit(1);
    }

    //Sinon, on renvoie l'adresse du section header souhaité

    return header->sectionHeaderTable[shndx];
}


Header* legolasReadFromFile(FILE* file) {
	//lecture du header
    Elf32_Ehdr fileHeader;
    fseek(file, 0, SEEK_SET);
    fread(&fileHeader, sizeof(fileHeader), 1, file);

    Header* header = malloc(sizeof(Header));

    setHeader(&fileHeader, header);

    header->sectionHeaderTable = malloc(sizeof(SectionHeader*) * header->shnum);

	//lecture de la table de sections
	int fileSectionHeaderTableSize = sizeof(Elf32_Shdr) * header->shnum;
    Elf32_Shdr* fileSectionHeaderTable = malloc(fileSectionHeaderTableSize);
    fseek(file, reverseEndian32(fileHeader.e_shoff), SEEK_SET);
    fread(fileSectionHeaderTable, fileSectionHeaderTableSize, 1, file);

    for (int i = 0; i < header->shnum; i++) {
        setSectionHeader(header, fileSectionHeaderTable, i, file);
    }

	//remplissage noms de header de section
	int shstrndx = reverseEndian16(fileHeader.e_shstrndx);
	SectionHeader* stringTableSectionHeader = header->sectionHeaderTable[shstrndx];

    for (int i = 0; i < header->shnum; i++) {
		SectionHeader* sectionHeader = header->sectionHeaderTable[i];
	    Elf32_Shdr* fileSectionHeader = &(fileSectionHeaderTable[i]);

		char* name = stringTableGetString(stringTableSectionHeader, reverseEndian32(fileSectionHeader->sh_name));
		sectionHeader->name = malloc(strlen(name));
        strcpy(sectionHeader->name, name);
    }

    for (int i=0; i < header->shnum; i++) {
        typeLastRawDataPartIfNeeded(header->sectionHeaderTable[i], header);
    }

	//suppression de la trable des chaines
	headerRemoveSectionHeader(header, stringTableSectionHeader);

	return header;
}

void typeLastRawDataPartIfNeeded(SectionHeader* currentSectionHeader, Header* header) {
    //On switch sur le type de section pour savoir si on doit ou non la traiter

    switch(currentSectionHeader->type) {
        case SHT_REL: {
            //On compte le nombre d'entrées de notre table de relocation courante

            uint32_t nbRelocationTableEntry = (currentSectionHeader->size / currentSectionHeader->entSize);

            //On ajoute les informations manquantes à chaque relocation entry

            for (int i=0; i < nbRelocationTableEntry; i++) {
                //On fait pointer une RealocationEntry sur le début de la rawData concernée

                RealocationEntry* relocationEntry = (RealocationEntry*) &(((char*) currentSectionHeader->rawData)[currentSectionHeader->entSize * i]);

                //On fait pointer le champ "sym" de notre relocation entry courante sur la bonne symbol entry

                currentSectionHeader->data.relocationTable[i]->sym = getSymboleTableEntryAddress(header, ELF32_R_SYM(reverseEndian32(relocationEntry->info)));
            }

            break;
        }

        case SHT_SYMTAB: {
            //On compte le nombre d'entrées de notre table de relocation courante

            uint32_t nbSymbolTableEntry = (currentSectionHeader->size / currentSectionHeader->entSize);

            //On ajoute les informations manquantes à chaque symbol entry

            for (int i=0; i < nbSymbolTableEntry; i++) {
                //On fait pointer une Elf32Sym sur le début de la rawData concernée

                Elf32Sym* elf32Sym = (Elf32Sym*) &(((char*) currentSectionHeader->rawData)[currentSectionHeader->entSize * i]);

                /*
                    On fait pointer le champ "sectionHeader" de notre symbol entry courante sur le bon
                    section header.
                */

                currentSectionHeader->data.symboleTable[i]->sectionHeader = getSectionHeaderAddress(header, reverseEndian16(elf32Sym->stShndx));

                /*
                    On fait pointer le champ "name" de notre symbol entry courante sur l'adresse de
                    son nom dans la string table
                */

                currentSectionHeader->data.symboleTable[i]->name = getSymbolTableEntryName(header, reverseEndian32(elf32Sym->stName));
            }

            break;
        }
    }
}

void symbolTableAddLocalEntry(SectionHeader* sectionHeader, SymboleTableEntry* symbolTableEntry, unsigned int index) {
	if (symbolTableEntry->bind == STB_LOCAL) {
        sectionHeader->data.symboleTable = realloc(sectionHeader->data.symboleTable, sizeof(SymboleTableEntry*) * (sectionHeader->nbEntry+1));
        sectionHeader->data.symboleTable[sectionHeader->nbEntry] = symbolTableEntry;
        sectionHeader->nbEntry++;
    	sectionHeader->size += sizeof(SymboleTableEntry);
	}
}

void symbolTableAddGlobalEntry(SectionHeader* sectionHeader, SymboleTableEntry* symbolTableEntry, unsigned int index) {
    if (symbolTableEntry->bind == STB_GLOBAL) {
		int noMatchInFirstSection = 0;

        for (int i=0; i < sectionHeader->nbEntry; i++) {
            SymboleTableEntry* currentSymbolTableEntry = sectionHeader->data.symboleTable[i];

            if (strcmp(symbolTableEntry->name, currentSymbolTableEntry->name) == 0) {
                noMatchInFirstSection = 1;
				
				if (currentSymbolTableEntry->type != SHN_UNDEF && symbolTableEntry->type != SHN_UNDEF) {
                    exit(1);
                }
                else if (currentSymbolTableEntry->type == SHN_UNDEF && symbolTableEntry->type != SHN_UNDEF) {
                    //On doit enlever l'élément du section header actuel et le remplacer par le nouveau
                
                    symboleTableRemoveEntry(sectionHeader, symbolTableEntry);

                    sectionHeader->data.symboleTable = realloc(sectionHeader->data.symboleTable, sizeof(SymboleTableEntry*) * (sectionHeader->nbEntry+1));
                    sectionHeader->data.symboleTable[sectionHeader->nbEntry] = symbolTableEntry;
                    sectionHeader->nbEntry++;
    				sectionHeader->size += sizeof(SymboleTableEntry);
                }
                else if (currentSymbolTableEntry->type != SHN_UNDEF && symbolTableEntry->type == SHN_UNDEF) {
                    //On ne doit rien faire car le bon élément est déjà dans la table, on ignore celui lu
                }
                else if (currentSymbolTableEntry->type == SHN_UNDEF && symbolTableEntry->type == SHN_UNDEF) {
                    //On ne fait rien car une seule occurence est nécessaire et on en a déjà une
                }
            }
        }

		if (noMatchInFirstSection == 0) {
             //On rajoute l'élément si le nom de l'élément actuel n'est pas dans la liste
             sectionHeader->data.symboleTable = realloc(sectionHeader->data.symboleTable, sizeof(SymboleTableEntry*) * (sectionHeader->nbEntry+1));
             sectionHeader->data.symboleTable[sectionHeader->nbEntry] = symbolTableEntry;
             sectionHeader->nbEntry++;	
    		 sectionHeader->size += sizeof(SymboleTableEntry);
		}
    }
}

void headerAddSection(Header* header, SectionHeader* sectionHeader) {
    header->shnum++;
    header->sectionHeaderTable = realloc(header->sectionHeaderTable, header->shnum * sizeof(SectionHeader*));
    header->sectionHeaderTable[header->shnum-1] = sectionHeader;
}

int headerGetIndexOfSectionHeader(Header* header, SectionHeader* sectionHeader) {
    int i = 0;

    while (i < header->shnum && header->sectionHeaderTable[i] != sectionHeader) {
        i++;
    }

    if (i >= header->shnum) {
        printf("Error : section header %p not found in header %p\n", sectionHeader, header);
        i = -1;
    }

    return i;
}

void headerRemoveSectionHeader(Header* header, SectionHeader* sectionHeader) {
		int i = headerGetIndexOfSectionHeader(header, sectionHeader);
		header->sectionHeaderTable[i] = header->sectionHeaderTable[header->shnum-1];
		header->shnum--;
		free(sectionHeader);
}

int symbolTableGetEntryIndex(SectionHeader* sectionHeader, SymboleTableEntry* symboleTableEntry) {
    int i = 0;

    while (i < sectionHeader->nbEntry && sectionHeader->data.symboleTable[i] != symboleTableEntry) {
        i++;
    }

    if (i >= sectionHeader->nbEntry) {
        printf("Error : symbol table entry %p not found in the symbol table", symboleTableEntry);
        i = -1;
    }

    return i;
}

void symboleTableRemoveEntry(SectionHeader* sectionHeader, SymboleTableEntry* symboleTableEntry) {
    int i = symbolTableGetEntryIndex(sectionHeader, symboleTableEntry);
    sectionHeader->data.symboleTable[i] = sectionHeader->data.symboleTable[sectionHeader->nbEntry-1];
    sectionHeader->nbEntry--;
    free(symboleTableEntry);
}

void stringTableAddString(SectionHeader* sectionHeader, char* string) {
    if (sectionHeader->type == SHT_STRTAB) {
		int newIndex = sectionHeader->size;
		sectionHeader->size += strlen(string) + 1;
		sectionHeader->data.stringTable = realloc(sectionHeader->data.stringTable, sectionHeader->size);
        strcpy(&(sectionHeader->data.stringTable[newIndex]), string);
		//sectionHeader->data.stringTable[sectionHeader->size - 1] = 0;
    } else {
        printf("Error : stringTableAddString called on a non SHT_STRTAB sectionHeader\n");
    }
}

char* stringTableGetString(SectionHeader* sectionHeader, int index) {
	char* string = NULL;

    if (sectionHeader->type == SHT_STRTAB) {
		string = &(sectionHeader->data.stringTable[index]);
    } else {
        printf("Error : stringTableAddString called on a non SHT_STRTAB sectionHeader\n");
    }

	return string;
}

int stringTableGetIndex(SectionHeader* sectionHeader, char* string) {
    int i = 0;

    if (sectionHeader->type == SHT_STRTAB) {
			while (i < sectionHeader->size && strcmp(&(sectionHeader->data.stringTable[i]), string) != 0) {
					i += strlen(&(sectionHeader->data.stringTable[i])) + 1;
			}

			if (i >= sectionHeader->size) {
					printf("Error : \"%s\" is not present in stringTable %p\n", string, sectionHeader);
					i = -1;
			}
    } else {
        printf("Error : stringTableGetIndex called on a non SHT_STRTAB sectionHeader\n");
		i = -1;
    }

    return i;
}

char* sectionHeaderGetRawData(SectionHeader* sectionHeader) {
    if (sectionHeader->type == SHT_STRTAB) {
        return sectionHeader->data.stringTable;
	} else {
		return (char*) sectionHeader->rawData;
    }
}

Elf32_Sym* sectionHeaderGetSymbolData(Header* header, SectionHeader* sectionHeader, SectionHeader* stringTable) {
	Elf32_Sym* symbolTable = malloc(sizeof(Elf32_Sym) * sectionHeader->nbEntry);
	
	for (int i=0; i < sectionHeader->nbEntry; i++) {
		Elf32_Sym* currentNewSymbolTableEntry = &(symbolTable[i]);
		SymboleTableEntry* currentOldSymbolTableEntry = sectionHeader->data.symboleTable[i];

		currentNewSymbolTableEntry->st_value = reverseEndian32(currentOldSymbolTableEntry->value);
		currentNewSymbolTableEntry->st_size = reverseEndian32(currentOldSymbolTableEntry->size);
		currentNewSymbolTableEntry->st_other = currentOldSymbolTableEntry->other;
		currentNewSymbolTableEntry->st_name = reverseEndian32(stringTableGetIndex(stringTable, currentOldSymbolTableEntry->name));
		currentNewSymbolTableEntry->st_info = ELF32_ST_INFO(currentOldSymbolTableEntry->bind, currentOldSymbolTableEntry->type);
		currentNewSymbolTableEntry->st_shndx = reverseEndian16(headerGetIndexOfSectionHeader(header, currentOldSymbolTableEntry->sectionHeader));
		printf("current ndx %i %i %s\n", reverseEndian16(currentNewSymbolTableEntry->st_shndx), currentOldSymbolTableEntry->sectionHeader->type, currentOldSymbolTableEntry->sectionHeader->name);
	}

	return symbolTable;
}
/*
Elf32_Rel* sectionHeaderGetUnexplicitRelocationData(SectionHeader* sectionHeader) {
}

Elf32_Rela* sectionHeaderGetExplicitRelocatonData(SectionHeader* sectionHeader) {
}
*/
uint32_t sectionHeaderGetEntSize(SectionHeader* sectionHeader) {
		return 1;
}

void legolasWriteToFile(Header* header, FILE* file) {
	for (int i=0; i < header->shnum; i++) {
		if (header->sectionHeaderTable[i]->type == SHT_STRTAB) {
			headerRemoveSectionHeader(header, header->sectionHeaderTable[i]);
			i--;
		}
	}

	//creation de la table des chaines des symbol table entry
    SectionHeader* stringTableSymbolTableEntry = malloc(sizeof(SectionHeader));
    stringTableSymbolTableEntry->name = ".strtab";
    stringTableSymbolTableEntry->type = SHT_STRTAB;
    stringTableSymbolTableEntry->size = 1;
	stringTableSymbolTableEntry->data.stringTable = malloc(1);
	stringTableSymbolTableEntry->data.stringTable[0] = 0;
    headerAddSection(header, stringTableSymbolTableEntry);

    //remplissage de la table des chaines des symbol table entry
    for (int i = 0; i < header->shnum; i++) {
	    SectionHeader* sectionHeader = header->sectionHeaderTable[i];

		if (sectionHeader->type == SHT_SYMTAB) {
	    	for (int j=0; j < sectionHeader->nbEntry; j++) {
				stringTableAddString(stringTableSymbolTableEntry, sectionHeader->data.symboleTable[j]->name);
			}
		}
    }

    //creation de la table des chaines des section header
    SectionHeader* stringTableSectionHeader = malloc(sizeof(SectionHeader));
    stringTableSectionHeader->name = ".shstrtab";
    stringTableSectionHeader->type = SHT_STRTAB;
    stringTableSectionHeader->size = 1;
	stringTableSectionHeader->data.stringTable = malloc(1);
	stringTableSectionHeader->data.stringTable[0] = 0;
    headerAddSection(header, stringTableSectionHeader);

    //remplissage de la table des chaines des section header
    for (int i = 0; i < header->shnum; i++) {
	    SectionHeader* sectionHeader = header->sectionHeaderTable[i];
	    stringTableAddString(stringTableSectionHeader, sectionHeader->name);
    }

	//creation du header
    Elf32_Ehdr fileHeader;
    fileHeader.e_ident[EI_MAG0] = ELFMAG0;
    fileHeader.e_ident[EI_MAG1] = ELFMAG1;
    fileHeader.e_ident[EI_MAG2] = ELFMAG2;
    fileHeader.e_ident[EI_MAG3] = ELFMAG3;
    fileHeader.e_ident[EI_CLASS] = header->indentClass;
    fileHeader.e_ident[EI_DATA] = header->indentData ;
    fileHeader.e_ident[EI_VERSION] = header->indentVersion;
    fileHeader.e_ident[EI_OSABI] = header->indentOSABI;
    fileHeader.e_ident[EI_ABIVERSION] = header->indentABIVersion;
    fileHeader.e_type = reverseEndian16(header->type);
    fileHeader.e_machine = reverseEndian16(header->machine);
    fileHeader.e_version = reverseEndian32(header->version);
    fileHeader.e_entry = reverseEndian32(header->entry);
    fileHeader.e_flags = reverseEndian32(header->flags);
    fileHeader.e_ehsize = reverseEndian16(sizeof(fileHeader));
    fileHeader.e_shnum = reverseEndian16(header->shnum);
	fileHeader.e_shentsize = reverseEndian16(sizeof(Elf32_Shdr));
    fileHeader.e_shstrndx = reverseEndian16(headerGetIndexOfSectionHeader(header, stringTableSectionHeader));
    fileHeader.e_phoff = 0;
    fileHeader.e_phentsize = 0;
    fileHeader.e_phnum = 0;

    //on laisse de la place dans le fichier pour le header
    fseek(file, sizeof(fileHeader), SEEK_SET);

    //creation de la table des sections
	int fileSectionHeaderTableSize = sizeof(Elf32_Shdr) * header->shnum;
    Elf32_Shdr* fileSectionHeaderTable = malloc(fileSectionHeaderTableSize);

    //ecriture des sections dans le fichier et memorisation des offsets
    for (int i = 0; i < header->shnum; i++) {
	    SectionHeader* sectionHeader = header->sectionHeaderTable[i];
	    Elf32_Shdr* fileSectionHeader = &(fileSectionHeaderTable[i]);

	    fileSectionHeader->sh_name = reverseEndian32(stringTableGetIndex(stringTableSectionHeader, sectionHeader->name));
	    fileSectionHeader->sh_type = reverseEndian32(sectionHeader->type);
	    fileSectionHeader->sh_flags = reverseEndian32(sectionHeader->flags);
	    fileSectionHeader->sh_addr = reverseEndian32(sectionHeader->addr);
	    fileSectionHeader->sh_link = reverseEndian32(sectionHeader->link);
	    fileSectionHeader->sh_info = reverseEndian32(sectionHeader->info);
	    fileSectionHeader->sh_addralign = reverseEndian32(sectionHeader->addrAlign);
	    fileSectionHeader->sh_size = reverseEndian32(sectionHeader->size);
	    fileSectionHeader->sh_entsize = reverseEndian32(sectionHeader->entSize);
	    fileSectionHeader->sh_offset = reverseEndian32(ftell(file));

		if (sectionHeader->type == SHT_SYMTAB) {
			fileSectionHeader->sh_size = reverseEndian32(sectionHeader->entSize * sectionHeader->nbEntry);
			fileSectionHeader->sh_link = reverseEndian32(headerGetIndexOfSectionHeader(header, stringTableSymbolTableEntry));
			fwrite(sectionHeaderGetSymbolData(header, sectionHeader, stringTableSymbolTableEntry), sectionHeader->size, 1, file);
		} else if (sectionHeader->type == SHT_REL) {
	//		fwrite(sectionHeaderGetUnexplicitRelocationData(sectionHeader), sectionHeader->size, 1, file);
		} else if (sectionHeader->type == SHT_RELA) {
	//		fwrite(sectionHeaderGetExplicitRelocationData(sectionHeader), sectionHeader->size, 1, file);
		} else {
			fwrite(sectionHeaderGetRawData(sectionHeader), sectionHeader->size, 1, file);
		}
    }

    //ecriture de la table des sections
    fileHeader.e_shoff = reverseEndian32(ftell(file));
    fwrite(fileSectionHeaderTable, fileSectionHeaderTableSize, 1, file);

    //eciture du header
    fseek(file, 0, SEEK_SET);
    fwrite(&fileHeader, sizeof(fileHeader), 1, file);
}
