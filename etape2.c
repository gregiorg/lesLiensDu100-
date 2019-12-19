#include "etape2.h"

ElfSecHeaderF** getTabElfSecHeader(FILE* file) {
	long int filePos = ftell(file);
	ElfSecHeader elfSecHeader;
	ElfHeaderF* header = getElfHeader(file);

	/*
		On récupère la position du début des headers de section,
		le nombre de sections, l'indice du header de la string
		table dans la liste des headers de section et la taille
		des headers de section à partir du ELF header renvoyé
		par l'étape 1.
	*/

	const uint32_t positionHeaderSection = header->shoff;
	const uint32_t nbSections = header->shnum;
	const uint32_t indexStringTableInSections = header->shstrndx;
	const uint32_t tailleHeaderSection = header->shentsize;

	//On se positionne au début des headers de section

	fseek(file, positionHeaderSection, SEEK_SET);

	//On récupère la position de la string table

	const uint32_t stringTableAddress = getAddressStringTable(positionHeaderSection, tailleHeaderSection, indexStringTableInSections, file);

	/*
		On alloue un tableau de pointeurs sur structure
		qui contiendra dans chaque case un pointeur sur
		une structure de header
	*/

	ElfSecHeaderF** finalHeader = malloc(sizeof(ElfSecHeaderF*) * nbSections);

	/*
		On lit et ajoute chaque header de section jusqu'au
		nombre de sections total.
	*/

	for (int i=0; i < nbSections; i++) {
		size_t codeRet = fread(&elfSecHeader, sizeof (elfSecHeader), 1, file);
		if(codeRet != 1) {
				if (feof(file)){
					printf("Erreur de lecture du fichier: fin de fichier inattendue\n");
					exit(EXIT_FAILURE);
				} else if (ferror(file)) {
					perror("Erreur de lecture du fichier");
					exit(EXIT_FAILURE);
				}
		}
		putCurrentHeader(elfSecHeader, stringTableAddress, file, i, finalHeader);
	}


    //afficherTabSecHeader(finalHeader, nbSections);

	fseek(file, filePos, SEEK_SET);
	return finalHeader;
}

void afficherTabSecHeader(ElfSecHeaderF** finalHeader, uint32_t nbSections) {
	//On affiche nos headers de section 1 par 1

	for (int i=0; i < nbSections; i++) {
		printf("Table numéro %i :\n", finalHeader[i]->indexTable);
		printf("	Indice du nom de la table : 0x%x\n", finalHeader[i]->indexName);
		printf(" 	Nom de la table : %s\n", finalHeader[i]->nameStr);
		printf("	Type int : 0x%x\n", finalHeader[i]->typeInt);
		printf("	Type str : %s\n", finalHeader[i]->typeStr);
		printf("	Flags : 0x%x\n", finalHeader[i]->flags);
		printf("	Adresse : 0x%x\n", finalHeader[i]->addr);
		printf("	Offset : 0x%x\n", finalHeader[i]->offset);
		printf("	Taille : 0x%x\n", finalHeader[i]->size);
		printf("	Link : 0x%x\n", finalHeader[i]->link);
		printf("	Info : 0x%x\n", finalHeader[i]->info);
		printf("	Adresse alignement : 0x%x\n", finalHeader[i]->addrAlign);
		printf("	Ent size : 0x%x\n", finalHeader[i]->entSize);

		printf("\n");
	}
}

void putCurrentHeader(ElfSecHeader elfSecHeader, uint32_t stringTableAddress, FILE* file, int i, ElfSecHeaderF** finalHeader) {
	//On alloue de la mémoire pour le header courant

	ElfSecHeaderF* currentHeader = malloc(sizeof(ElfSecHeaderF));

	//On remplit le header courant puis on l'ajoute au tableau de header "finalHeader"

	currentHeader->indexTable = i;
	currentHeader->indexName = reverseEndian32(elfSecHeader.name);
	currentHeader->typeInt = reverseEndian32(elfSecHeader.type);
	currentHeader->flags = reverseEndian32(elfSecHeader.flags);
	currentHeader->addr = reverseEndian32(elfSecHeader.addr);
	currentHeader->offset = reverseEndian32(elfSecHeader.offset);
	currentHeader->size = reverseEndian32(elfSecHeader.size);
	currentHeader->link = reverseEndian32(elfSecHeader.link);
	currentHeader->info = reverseEndian32(elfSecHeader.info);
	currentHeader->addrAlign = reverseEndian32(elfSecHeader.addrAlign);
	currentHeader->entSize = reverseEndian32(elfSecHeader.entSize);

	currentHeader->nameStr = showName(reverseEndian32(elfSecHeader.name), stringTableAddress, file);
	currentHeader->typeStr = showType(reverseEndian32(elfSecHeader.type));

	finalHeader[i] = currentHeader;
}
