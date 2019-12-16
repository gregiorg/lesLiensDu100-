#include <stdio.h>
#include <stdint.h>
#include "util.h"
#include <string.h>

void afficherHeader(Elf_SecHeader elf_secHeader, FILE* f);
void showType(uint32_t type);
void showName(uint32_t name, FILE *f);
void getAddressStringTable(uint32_t tailleHeader, uint32_t positionStringTable);

int main (int argc, char** argv)
{
	FILE* f = fopen(argv[1], "r");

	Elf_SecHeader elf_secHeader;

	//on récupèrera tailleHeader de l'étape 1 (720 = taille du header de exemple 0)

	const uint32_t tailleHeader = 720;

	//on récupèrera nbSections de l'étape 1 (15 = nb sections de exemple 0)

	const uint32_t nbSections = 15;

	fseek(f, tailleHeader, SEEK_SET);

	const uint32_t stringTableAddress = getAddressStringTable();

	for (int i=0; i < nbSections; i++)
	{
		fread(&elf_secHeader, sizeof (elf_secHeader), 1, f);

		afficherHeader(elf_secHeader, f);
	}
}

uint32_t getAddressStringTable()
{

}

void afficherHeader(Elf_SecHeader elf_secHeader, FILE* f)
{

	printf("Indice du nom de la table : 0x%x\n", reverse_endian_32(elf_secHeader.name));
	
	const int currentPos = ftell(f);

	showName(reverse_endian_32(elf_secHeader.name), f);
	
	fseek(f, currentPos, SEEK_SET);
	
	showType(reverse_endian_32(elf_secHeader.type));
	printf("Flags : 0x%x\n", reverse_endian_32(elf_secHeader.flags));
	printf("Adresse : 0x%x\n", reverse_endian_32(elf_secHeader.addr));
	printf("Offset : 0x%x\n", reverse_endian_32(elf_secHeader.offset));
	printf("Taille : 0x%x\n", reverse_endian_32(elf_secHeader.size));
	printf("Link : 0x%x\n", reverse_endian_32(elf_secHeader.link));
	printf("Info : 0x%x\n", reverse_endian_32(elf_secHeader.info));
	printf("Adresse alignement : 0x%x\n", reverse_endian_32(elf_secHeader.addrAlign));
	printf("Ent size : 0x%x\n", reverse_endian_32(elf_secHeader.entSize));

	printf("\n");
}

void showName(uint32_t name, FILE* f)
{
	const uint32_t decalage = 0x250 + name;

	fseek(f, decalage, SEEK_SET);

	char test[50];

	fread(test, sizeof(test), 1, f);

	printf("Nom de la table : %s\n", test);
}

void showType(uint32_t type)
{
	char name[15];

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

	printf("Type : %s\n", name);
}
