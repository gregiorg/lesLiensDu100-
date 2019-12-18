/*
ELF Loader - chargeur/implanteur d'exécutables au format ELF à but pédagogique
Copyright (C) 2012 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/

#include "util.h"

int is_big_endian() {
    static uint32_t one = 1;
    return ((* (uint8_t *) &one) == 0);
}

uint32_t reverseEndian32(uint32_t val) {
    uint8_t* tabIn = (uint8_t*) &val;
    uint8_t tabOut[sizeof (uint32_t)];

    for (int i = 0; i < sizeof (uint32_t); i++)
        tabOut[i] = tabIn[sizeof (uint32_t) - 1 - i];

    return *((uint32_t*) tabOut);
}

uint16_t reverseEndian16(uint16_t val) {
    uint8_t* tabIn = (uint8_t*) &val;
    uint8_t tabOut[sizeof (uint16_t)];

    for (int i = 0; i < sizeof (uint16_t); i++)
        tabOut[i] = tabIn[sizeof (uint16_t) - 1 - i];

    return *((uint16_t*) tabOut);
}

ElfHeaderF* getElfHeader(FILE* file) {
    ElfHeaderF* elfHeaderF = malloc(sizeof(ElfHeaderF));
    long int filePos = ftell(file);
    fseek(file, 0, SEEK_SET);

    ElfHeader elfHeader;
    if (fread(&elfHeader, sizeof (elfHeader), 1, file)){
      //cool
    }

    switch (elfHeader.indentClass) {
        case 0x0:
            elfHeaderF->indentClass = "invalide";
            break;
        case 0x1:
            elfHeaderF->indentClass = "32 bits";
            break;
        case 0x2:
            elfHeaderF->indentClass = "64 bits";
            break;
        default:
            elfHeaderF->indentClass = "inconnu";
    }

    switch (elfHeader.indentData) {
        case 0x0:
            elfHeaderF->indentData = "invalide";
            break;
        case 0x1:
            elfHeaderF->indentData = "petits";
            break;
        case 0x2:
            elfHeaderF->indentData = "gros";
            break;
        default:
            elfHeaderF->indentData = "inonnu";
    }

    switch (reverseEndian16(elfHeader.type)) {
        case 0x0:
            elfHeaderF->type = "aucun";
            break;
        case 0x1:
            elfHeaderF->type = "relogeable";
            break;
        case 0x2:
            elfHeaderF->type = "executable";
            break;
        default:
            elfHeaderF->type = "inonnu";
    }

    switch (reverseEndian16(elfHeader.machine)) {
        case 0x0:
            elfHeaderF->machine = "aucune";
            break;
        case 0x28:
            elfHeaderF->machine = "ARM";
            break;
        default:
            elfHeaderF->machine = "inconnu";
    }

    elfHeaderF->shoff = reverseEndian32(elfHeader.shoff);
    elfHeaderF->shnum = reverseEndian16(elfHeader.shnum);
    elfHeaderF->shentsize = reverseEndian16(elfHeader.shentsize);
    elfHeaderF->shsize = elfHeaderF->shnum * elfHeaderF->shentsize;
    elfHeaderF->shstrndx = reverseEndian16(elfHeader.shstrndx);

    elfHeaderF->ehsize = reverseEndian16(elfHeader.ehsize);

    fseek(file, filePos, SEEK_SET);
    return elfHeaderF;
}

char* getRelocationName(uint32_t relocationCode) {
	char* name = malloc(200);
	
	switch (relocationCode) {
		case R_ARM_ABS32:
			strcpy(name, "R_ARM_ABS32");
			break;

		default:
			strcpy(name, "Unknown type. Please add this type to the util.h enum and to the util.c getRelocationName function")
			break;
	}

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

		case SHT_LOUSER:
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


char* showName(uint32_t indexName, uint32_t stringTableAddress, FILE* f) {
	const int currentPos = ftell(f);

	const uint32_t decalage = stringTableAddress + indexName;

	fseek(f, decalage, SEEK_SET);

	char* name = malloc(50);

	fread(name, 50, 1, f);

	fseek(f, currentPos, SEEK_SET);

	return name;
}

uint32_t getAddressStringTable(uint32_t tailleHeaderSection, uint32_t positionStringTable, FILE* f) {
	const int currentPos = ftell(f);

	fseek(f, tailleHeaderSection * positionStringTable, SEEK_CUR);

	ElfSecHeader elfSecHeader2;

	fread(&elfSecHeader2, sizeof (elfSecHeader2), 1, f);

	fseek(f, currentPos, SEEK_SET);

	return reverseEndian32(elfSecHeader2.offset);
}
