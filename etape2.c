#include "etape2.h"

ElfSecHeaderF** etape2(FILE* f) {
	ElfSecHeader elfSecHeader;
	ElfHeaderF* header = getElfHeader(f);

	const uint32_t positionHeaderSection = header->shoff;
	const uint32_t nbSections = header->shnum;
	const uint32_t positionStringTable = header->shstrndx;
	const uint32_t tailleHeaderSection = header->shentsize;

	fseek(f, positionHeaderSection, SEEK_SET);

	const uint32_t stringTableAddress = getAddressStringTable(tailleHeaderSection, positionStringTable, f);

	ElfSecHeaderF** finalHeader = malloc(sizeof(ElfSecHeaderF*) * nbSections);

	for (int i=0; i < nbSections; i++) {
		fread(&elfSecHeader, sizeof (elfSecHeader), 1, f);
		putCurrentHeader(elfSecHeader, stringTableAddress, f, i, finalHeader);
	}

  	//afficheFinal(finalHeader, nbSections);

	return finalHeader;
}

void afficheFinal(ElfSecHeaderF** finalHeader, uint32_t nbSections) {
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

void putCurrentHeader(ElfSecHeader elfSecHeader, uint32_t stringTableAddress, FILE* f, int i, ElfSecHeaderF** finalHeader) {
	ElfSecHeaderF* currentHeader = malloc(sizeof(ElfSecHeaderF));

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

	currentHeader->nameStr = showName(reverseEndian32(elfSecHeader.name), stringTableAddress, f);
	currentHeader->typeStr = showType(reverseEndian32(elfSecHeader.type));

	finalHeader[i] = currentHeader;
}
