#include "etape2.h"

Elf_SecHeaderF* etape2(char* fname)
{
	FILE* f = fopen(fname, "r");

	Elf_SecHeader elf_secHeader;
	ElfHeaderF * header = get_elf_header(f);

	uint32_t positionHeaderSection = header->shoff;
	uint32_t nbSections = header->shnum;
	uint32_t positionStringTable = header->shstrndx;
	uint32_t tailleHeaderSection = header->shentsize;

	fseek(f, positionHeaderSection, SEEK_SET);

	const uint32_t stringTableAddress = getAddressStringTable(tailleHeaderSection, positionStringTable, f);

	// printf("Offset string table : 0x%x\n\n", stringTableAddress);

	Elf_SecHeaderF * finalHeader = malloc(sizeof(Elf_SecHeaderF) * nbSections);

	for (int i=0; i < nbSections; i++)
	{
		fread(&elf_secHeader, sizeof (elf_secHeader), 1, f);
		putCurrentHeader(elf_secHeader, stringTableAddress, f, i, finalHeader);
	}

	return finalHeader;

  // afficheFinal(finalHeader, nbSections);
}

void afficheFinal(Elf_SecHeaderF *finalHeader, uint32_t nbSections)
{
	for (int i=0; i < nbSections; i++)
	{
		printf("Table numéro %i :\n", finalHeader[i].indexTable);
		printf("	Indice du nom de la table : 0x%x\n", finalHeader[i].indexName);
		printf(" 	Nom de la table : %s\n", finalHeader[i].nameStr);
		printf("	Type int : 0x%x\n", finalHeader[i].typeInt);
		printf("	Type str : %s\n", finalHeader[i].typeStr);
		printf("	Flags : 0x%x\n", finalHeader[i].flags);
		printf("	Adresse : 0x%x\n", finalHeader[i].addr);
		printf("	Offset : 0x%x\n", finalHeader[i].offset);
		printf("	Taille : 0x%x\n", finalHeader[i].size);
		printf("	Link : 0x%x\n", finalHeader[i].link);
		printf("	Info : 0x%x\n", finalHeader[i].info);
		printf("	Adresse alignement : 0x%x\n", finalHeader[i].addrAlign);
		printf("	Ent size : 0x%x\n", finalHeader[i].entSize);

		printf("\n");
	}
}

uint32_t getAddressStringTable(uint32_t tailleHeaderSection, uint32_t positionStringTable, FILE* f)
{
	const int currentPos = ftell(f);

	fseek(f, tailleHeaderSection * positionStringTable, SEEK_CUR);

	Elf_SecHeader elf_secHeader2;

	fread(&elf_secHeader2, sizeof (elf_secHeader2), 1, f);

	fseek(f, currentPos, SEEK_SET);

	return reverse_endian_32(elf_secHeader2.offset);
}

void putCurrentHeader(Elf_SecHeader elf_secHeader, uint32_t stringTableAddress, FILE* f, int i, Elf_SecHeaderF *finalHeader)
{
	Elf_SecHeaderF currentHeader;

	currentHeader.indexTable = i;
	currentHeader.indexName = reverse_endian_32(elf_secHeader.name);
	currentHeader.typeInt = reverse_endian_32(elf_secHeader.type);
	currentHeader.flags = reverse_endian_32(elf_secHeader.flags);
	currentHeader.addr = reverse_endian_32(elf_secHeader.addr);
	currentHeader.offset = reverse_endian_32(elf_secHeader.offset);
	currentHeader.size = reverse_endian_32(elf_secHeader.size);
	currentHeader.link = reverse_endian_32(elf_secHeader.link);
	currentHeader.info = reverse_endian_32(elf_secHeader.info);
	currentHeader.addrAlign = reverse_endian_32(elf_secHeader.addrAlign);
	currentHeader.entSize = reverse_endian_32(elf_secHeader.entSize);

	currentHeader.nameStr = showName(reverse_endian_32(elf_secHeader.name), stringTableAddress, f);
	currentHeader.typeStr = showType(reverse_endian_32(elf_secHeader.type));

	finalHeader[i] = currentHeader;
}

char * showName(uint32_t indexName, uint32_t stringTableAddress, FILE* f)
{
	const int currentPos = ftell(f);

	const uint32_t decalage = stringTableAddress + indexName;

	fseek(f, decalage, SEEK_SET);

	char * name = malloc(50);

	fread(name, 50, 1, f);

	fseek(f, currentPos, SEEK_SET);

	return name;
}
char * showType(uint32_t type)
{
	char * typeStr = malloc(15);

	switch (type)
	{
		case 0:
			strcpy(typeStr, "NULL");
			break;

		case 1:
			strcpy(typeStr, "PROGBITS");
			break;

		case 2:
			strcpy(typeStr, "SYMTAB");
			break;

		case 3:
			strcpy(typeStr, "STRTAB");
			break;

		case 4:
			strcpy(typeStr, "RELA");
			break;

		case 5:
			strcpy(typeStr, "HASH");
			break;

		case 6:
			strcpy(typeStr, "DYNAMIC");
			break;

		case 7:
			strcpy(typeStr, "NOTE");
			break;

		case 8:
			strcpy(typeStr, "NOBITS");
			break;

		case 9:
			strcpy(typeStr, "REL");
			break;

		case 10:
			strcpy(typeStr, "SHLIB");
			break;

		case 11:
			strcpy(typeStr, "DYNSYM");
			break;

		case 0x70000000:
			strcpy(typeStr, "LOPROC");
			break;

		case 0x70000003:
			strcpy(typeStr, "ARM_ATTRIBUTES");
			break;

		case 0x7fffffff:
			strcpy(typeStr, "HIPROC");
			break;

		case 0x80000000:
			strcpy(typeStr, "LOUSER");
			break;

		case 0xffffffff:
			strcpy(typeStr, "HIUSER");
			break;

		default:
			strcpy(typeStr, "UNKNOWN SECTION TYPE");
			break;
	}

	return typeStr;
}
