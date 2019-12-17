#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "util.h"
#include <string.h>

void putCurrentHeader(Elf_SecHeader elf_secHeader, uint32_t stringTableAddress, FILE* f, int i, Elf_SecHeaderF * elf_secHeaderF);
char * showType(uint32_t type);
char * showName(uint32_t name, uint32_t stringTableAddress, FILE *f);
uint32_t getAddressStringTable(uint32_t tailleHeaderSection, uint32_t positionStringTable, FILE* f);
Elf_SecHeaderF returnFinalHeader(Elf_SecHeaderF elf_secHeaderF);
void afficheFinal(Elf_SecHeaderF * finalHeader, uint32_t nbSections);

Elf_SecHeaderF etape2(char* fname)
{
	FILE* f = fopen(fname, "r");

	Elf_SecHeader elf_secHeader;

	//on récupèrera tailleHeader de l'étape 1 (720 = taille du header de exemple 0)

	const uint32_t tailleHeader = 720;

	//on récupèrera nbSections de l'étape 1 (15 = nb sections de exemple 0)

	const uint32_t nbSections = 15;

	//on récupèrera positionStringTable de l'étape 1 (12 = position de la string table de exemple 0)

	const uint32_t positionStringTable = 12;

	//on récupèrera tailleEnteteSection de l'étape 1 (40 = taille des entetes de section de l'exemple 0)

	const uint32_t tailleHeaderSection = 40;

	fseek(f, tailleHeader, SEEK_SET);

	const uint32_t stringTableAddress = getAddressStringTable(tailleHeaderSection, positionStringTable, f);

	printf("Offset string table : 0x%x\n\n", stringTableAddress);

	Elf_SecHeaderF * finalHeader = malloc(sizeof(Elf_SecHeaderF) * nbSections);

	for (int i=0; i < nbSections; i++)
	{
		fread(&elf_secHeader, sizeof (elf_secHeader), 1, f);

//		printf("Table numéro %i :\n", i);
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

	/*
	printf("	Indice du nom de la table : 0x%x\n", reverse_endian_32(elf_secHeader.name));
	showName(reverse_endian_32(elf_secHeader.name), stringTableAddress, f);
	showType(reverse_endian_32(elf_secHeader.type));
	printf("	Flags : 0x%x\n", reverse_endian_32(elf_secHeader.flags));
	printf("	Adresse : 0x%x\n", reverse_endian_32(elf_secHeader.addr));
	printf("	Offset : 0x%x\n", reverse_endian_32(elf_secHeader.offset));
	printf("	Taille : 0x%x\n", reverse_endian_32(elf_secHeader.size));
	printf("	Link : 0x%x\n", reverse_endian_32(elf_secHeader.link));
	printf("	Info : 0x%x\n", reverse_endian_32(elf_secHeader.info));
	printf("	Adresse alignement : 0x%x\n", reverse_endian_32(elf_secHeader.addrAlign));
	printf("	Ent size : 0x%x\n", reverse_endian_32(elf_secHeader.entSize));

	printf("\n");
	*/
}

char * showName(uint32_t indexName, uint32_t stringTableAddress, FILE* f)
{
	const int currentPos = ftell(f);

	const uint32_t decalage = stringTableAddress + indexName;

	fseek(f, decalage, SEEK_SET);

	char * test = malloc(50);

	fread(test, 50, 1, f);

//	printf("	Nom de la table : %s\n", test);

	fseek(f, currentPos, SEEK_SET);

	return test;
}
char * showType(uint32_t type)
{
	char * name = malloc(15);

	switch (type)
	{
		case 0:
			strcpy(name, "NULL");
			break;

		case 1:
			strcpy(name, "PROGBITS");
			break;

		case 2:
			strcpy(name, "SYMTAB");
			break;

		case 3:
			strcpy(name, "STRTAB");
			break;

		case 4:
			strcpy(name, "RELA");
			break;

		case 5:
			strcpy(name, "HASH");
			break;

		case 6:
			strcpy(name, "DYNAMIC");
			break;

		case 7:
			strcpy(name, "NOTE");
			break;

		case 8:
			strcpy(name, "NOBITS");
			break;

		case 9:
			strcpy(name, "REL");
			break;

		case 10:
			strcpy(name, "SHLIB");
			break;

		case 11:
			strcpy(name, "DYNSYM");
			break;

		case 0x70000000:
			strcpy(name, "LOPROC");
			break;

		case 0x70000003:
			strcpy(name, "ARM_ATTRIBUTES");
			break;

		case 0x7fffffff:
			strcpy(name, "HIPROC");
			break;

		case 0x80000000:
			strcpy(name, "LOUSER");
			break;

		case 0xffffffff:
			strcpy(name, "HIUSER");
			break;

		default:
			strcpy(name, "ERREUR");
			break;
	}

	//printf("	Type : %s\n", name);
	return name;
}
