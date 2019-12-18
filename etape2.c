#include "etape2.h"

enum={SHT_NULL=0, SHT_PROGBITS=1, SHT_SYMTAB=2, SHT_STRTAB=3, SHT_RELA=4, SHT_HASH=5, SHT_DYNAMIC=6, SHT_NOTE=7, SHT_NOBITS=8, SHT_REL=9, SHT_SHLIB=10, SHT_DYNSYM=11, SHT_LOPROC=0x70000000, SHT_ARM_ATTRIBUTES=0x70000003, SHT_HIPROC=0x7fffffff, SHT_LOUSER=0x80000000, SHT_HIUSER=0xffffffff}

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

uint32_t getAddressStringTable(uint32_t tailleHeaderSection, uint32_t positionStringTable, FILE* f) {
	const int currentPos = ftell(f);

	fseek(f, tailleHeaderSection * positionStringTable, SEEK_CUR);

	ElfSecHeader elfSecHeader2;

	fread(&elfSecHeader2, sizeof (elfSecHeader2), 1, f);

	fseek(f, currentPos, SEEK_SET);

	return reverseEndian32(elfSecHeader2.offset);
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

char* showName(uint32_t indexName, uint32_t stringTableAddress, FILE* f) {
	const int currentPos = ftell(f);

	const uint32_t decalage = stringTableAddress + indexName;

	fseek(f, decalage, SEEK_SET);

	char* name = malloc(50);

	fread(name, 50, 1, f);

	fseek(f, currentPos, SEEK_SET);

	return name;
}

char* showType(uint32_t type) {
	char* typeStr = malloc(15);

	switch (type)
	{
		case SHT_NULL:
			strcpy(typeStr, "NULL");
			break;

		case SHT_PROGBITS:
			strcpy(typeStr, "PROGBITS");
			break;

		case SHT_SYMTAB:
			strcpy(typeStr, "SYMTAB");
			break;

		case SHT_STRTAB:
			strcpy(typeStr, "STRTAB");
			break;

		case SHT_RELA:
			strcpy(typeStr, "RELA");
			break;

		case SHT_HASH:
			strcpy(typeStr, "HASH");
			break;

		case SHT_DYNAMIC:
			strcpy(typeStr, "DYNAMIC");
			break;

		case SHT_NOTE:
			strcpy(typeStr, "NOTE");
			break;

		case SHT_NOBITS:
			strcpy(typeStr, "NOBITS");
			break;

		case SHT_REL:
			strcpy(typeStr, "REL");
			break;

		case SHT_SHLIB:
			strcpy(typeStr, "SHLIB");
			break;

		case SHT_DYNSYM:
			strcpy(typeStr, "DYNSYM");
			break;

		case SHT_LOPROC:
			strcpy(typeStr, "LOPROC");
			break;

		case SHT_ARM_ATTRIBUTES:
			strcpy(typeStr, "ARM_ATTRIBUTES");
			break;

		case SHT_HIPROC:
			strcpy(typeStr, "HIPROC");
			break;

		case SHT_ARM_ATTRIBUTES:
			strcpy(typeStr, "LOUSER");
			break;

		case SHT_HIUSER:
			strcpy(typeStr, "HIUSER");
			break;

		default:
			strcpy(typeStr, "UNKNOWN SECTION TYPE");
			break;
	}

	return typeStr;
}
