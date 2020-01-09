#include "legolas.h"

void setSectionHeader(Header* header, Elf32_Shdr* fileSectionHeaderTable, int i, FILE* file) {
	//On ajoute toutes les informations du section header courant
	
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

	//On type le plus possible la raw data du section header actuel si besoin

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
                currentRelocationTableEntry->offset = reverseEndian32(relocationEntry->offset);

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

SectionHeader* getSectionHeaderAddress(Header* header, uint16_t shndx) {
    /*
        Si l'indice transmis est supérieur au nombre de sections total,
        on quitte le programme avec une erreur.
    */

    if (shndx > header->shnum - 1) {
        printf("The transmitted index is greater than the total number of sections.");
        exit(1);
    }

    //Sinon, on renvoie l'adresse du section header souhaité

    return header->sectionHeaderTable[shndx];
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

char* sectionHeaderGetRawData(SectionHeader* sectionHeader) {
	/*
		On renvoie un pointeur sur la string table si le section header est de type string table,
		sinon on renvoie un pointeur de type char sur la rawData.
	*/
   	
    if (sectionHeader->type == SHT_STRTAB) {
        return sectionHeader->data.stringTable;
	} else {
		return (char*) sectionHeader->rawData;
    }
}

uint32_t sectionHeaderGetEntSize(SectionHeader* sectionHeader) {
		return 1;
}

