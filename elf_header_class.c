#include "elf_header_class.h"
#include <stdlib.h>
#include <elf.h>

void setHeader(ElfHeader elfHeader, Header* header) {
	//On récupère les informations du elfHeader et on les	ajoute au header.

	header->indentClass = elfHeader.indentClass;
    header->indentData = elfHeader.indentData;
    header->indentVersion = elfHeader.indentVersion;
    header->type = reverseEndian16(elfHeader.type);
    header->machine = reverseEndian16(elfHeader.machine);
    header->version = reverseEndian32(elfHeader.version);
    header->entry = reverseEndian32(elfHeader.entry);
    header->flags = reverseEndian32(elfHeader.flags);
	header->nbSections = reverseEndian16(elfHeader.shnum);
}

void setSectionHeader(Header* header, ElfSecHeader* elfSecHeaders, int i, FILE* file) {
	//On malloc un "objet" SectionHeader

	SectionHeader* currentSectionHeader = malloc(sizeof(SectionHeader));
	
	//On recrée un ElfSecHeader avec les informations du i-ème elfSecHeader
	
	ElfSecHeader currentElfSecHeader = elfSecHeaders[i];

	//On remplit notre "objet" SectionHeader

	currentSectionHeader->indexTable = i;
	currentSectionHeader->type = reverseEndian32(currentElfSecHeader.type);
	currentSectionHeader->flags = reverseEndian32(currentElfSecHeader.flags);
	currentSectionHeader->addr = reverseEndian32(currentElfSecHeader.addr);
	currentSectionHeader->link = reverseEndian32(currentElfSecHeader.link);
	currentSectionHeader->info = reverseEndian32(currentElfSecHeader.info);
	currentSectionHeader->addrAlign = reverseEndian32(currentElfSecHeader.addrAlign);
	currentSectionHeader->entSize = reverseEndian32(currentElfSecHeader.entSize);
	currentSectionHeader->size = reverseEndian32(currentElfSecHeader.size);

	/*
		On malloc l'attribut "rawData" de notre SectionHeader
		de la taille de la section courante (= la taille de la data).
	*/

	currentSectionHeader->rawData = malloc(currentSectionHeader->size);
	
	//On se décale dans le fichier de l'offset de la section courante
	
	fseek(file, reverseEndian32(currentElfSecHeader.offset), SEEK_SET);

	/*
		On fait pointer notre "rawData" sur les (tailleSection) premiers
		octets du fichier à partir de la position courante.
	*/
		
	fread(currentSectionHeader->rawData, currentSectionHeader->size, 1, file);

	/*
		On type notre rawData au maximum (certaines informations nécessitent
		un second passage) pour les sections spéciales (string tables, relocation
		table, symbol table).
	*/

	typeFirstRawDataPartIfNeeded(currentSectionHeader, header);	

	/*
		On fait pointer la section courante vers le section header que l'on vient
		de remplir
	*/
		
	header->sections[i] = currentSectionHeader;
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

char* getSymbolTableEntryName(Header* header, uint32_t indexName) {
	int indexStringTable = 0;
	
	/*
		Tant que le nom de la section qu'on analyse est différent de ".strtab" (i.e. le nom
		de la string table des symboles), on incrément notre indice.
		En sortie de while, indexStringTable = indice de la string table des symboles.
	*/

	while(indexStringTable < header->nbSections && (strcmp(header->sections[indexStringTable]->name, ".strtab") != 0)) {
    	indexStringTable++;
	}

	/*
		On vérifie qu'on soit bien en présence d'une string table pour symboles.
		Si ce n'est pas le cas, on quitte le programme avec une erreur.
	*/

	if (strcmp(header->sections[indexStringTable]->name, ".strtab") != 0) {
		printf("The string table for symbols was not found. Please make sure that your ELF file is not corrupted.");
		exit(1);
	}

	return ((header->sections[indexStringTable]->data.stringTable) + indexName);
}

SymboleTableEntry* getSymboleTableEntryAddress(Header* header, uint32_t info) {
	unsigned int i = 0;

	/*
		Tant que le type de section est différent du type "symbol table", on incrémente i.
		En sortie de while, i = indice de la symbol table.
	*/	

	while (i < header->nbSections && header->sections[i]->type != SHT_SYMTAB) {
		i++;
	}

	/*
	 	On vérifie qu'on soit bien en présence d'une symbol table.
		Si ce n'est pas le cas, on quitte le programme avec une erreur.
	*/

	if (header->sections[i]->type != SHT_SYMTAB) {
		printf("The symbol table was not found. Please make sure that your ELF file is not corrupted.");
		exit(1);
	}

	return header->sections[i]->data.symboleTable[info];
}

SectionHeader* getSectionHeaderAddress(Header* header, uint16_t shndx) {
	/*
		Si l'indice transmis est supérieur au nombre de sections total,
		on quitte le programme avec une erreur.
	*/
	
	if (shndx > header->nbSections - 1) {
		printf("L'indice transmis est supérieur au nombre de sections total");
		exit(1);
	}

	//Sinon, on renvoie l'adresse du section header souhaité

	return header->sections[shndx];	
}

Header* headerFromFile(FILE* file) {
	// On crée un elfHeader et on lit dans le fichier pour le remplir
	
    ElfHeader elfHeader;
    fseek(file, 0, SEEK_SET);
    fread(&elfHeader, sizeof(ElfHeader), 1, file);

	//On malloc un "objet" header qui sera la base de notre arborescence objet

    Header* header = malloc(sizeof(Header));
	
	//On remplit le header avec les informations du elfHeader

    setHeader(elfHeader, header);

	//On malloc un tableau de elfSecHeaders, puis on le remplit en lisant dans le fichier

    ElfSecHeader* elfSecHeaders = malloc(sizeof(ElfSecHeader) * header->nbSections);
    fseek(file, reverseEndian32(elfHeader.shoff), SEEK_SET);
    fread(elfSecHeaders, sizeof(ElfSecHeader), header->nbSections, file);

	//On malloc un tableau de pointeurs sur sections

    header->sections = malloc(sizeof(void*) * header->nbSections);

	/*
		On remplit les section headers au maximum (le nom de chaque section
		nécessite un second passage)
	*/

    for (int i = 0; i < header->nbSections; i++) {
        setSectionHeader(header, elfSecHeaders, i, file);
    }

	//On récupère l'index de la string table, puis la string table

	int shstrndx = reverseEndian16(elfHeader.shstrndx);
    char* stringTable = (char*) header->sections[shstrndx]->rawData;

	//On finit de remplir les sections en renseignant leur nom

    for (int i = 0; i < header->nbSections; i++) {
		SectionHeader* currentSectionHeader = header->sections[i];
		ElfSecHeader currentElfSecHeader = elfSecHeaders[i];
        currentSectionHeader->name = stringTable + reverseEndian32(currentElfSecHeader.name);
	}

	/*
		On transforme la donnée brute de certaines tables (relocation
		table, string tables, symbol table) en des structures
		contenant des informations plus lisibles, récupérables plus
		facilement
	*/

	for (int i=0; i < header->nbSections; i++) {
		typeLastRawDataPartIfNeeded(header->sections[i], header);
	}

	//On return finalement un pointeur sur notre header remplit

	return header;
}
