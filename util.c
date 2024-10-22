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
    if (programsEndian == 0) {
      fprintf(stderr,"ERROR : taille des indiens non détectée\n");
      exit(EXIT_FAILURE);
    }

    if ((is_big_endian() && programsEndian == LITTLE_END) ||
    (!is_big_endian() && programsEndian == BIG_END)) {  // testing if the programms and the os endianness is different
      uint8_t* tabIn = (uint8_t*) &val;
      uint8_t tabOut[sizeof (uint32_t)];

      for (int i = 0; i < sizeof (uint32_t); i++)
          tabOut[i] = tabIn[sizeof (uint32_t) - 1 - i];

      return *((uint32_t*) tabOut);
    } else {
      return val;
    }
}

uint16_t reverseEndian16(uint16_t val) {
  if (programsEndian == 0) {
    fprintf(stderr,"ERROR : taille des indiens non detecter\n");
    exit(EXIT_FAILURE);
  }

  if ((is_big_endian() && programsEndian == LITTLE_END) ||
  (!is_big_endian() && programsEndian == BIG_END)) {  // testing if the programms and the os endianness is different
    uint8_t* tabIn = (uint8_t*) &val;
    uint8_t tabOut[sizeof (uint16_t)];

    for (int i = 0; i < sizeof (uint16_t); i++)
        tabOut[i] = tabIn[sizeof (uint16_t) - 1 - i];

    return *((uint16_t*) tabOut);
  } else {
    return val;
  }
}


char* getRelocationName(uint32_t relocationCode) {
	char* name = malloc(200);

	switch (relocationCode) {
		case R_ARM_ABS32:
			strcpy(name, "R_ARM_ABS32");
			break;

		default:
			strcpy(name, "Unknown type. Please add this type to the util.h enum and to the util.c getRelocationName function");
			break;
	}

	return name;
}

char* showType(uint32_t type) {
	char* typeStr = malloc(30);

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



char* showName(uint32_t indexName, uint32_t stringTableAddress, FILE* file) {
	const int currentPos = ftell(file);

	const uint32_t decalage = stringTableAddress + indexName;

	fseek(file, decalage, SEEK_SET);

	char* name = malloc(50);

  freadChar(name, 50, 1, file);

	fseek(file, currentPos, SEEK_SET);

	return name;
}

uint32_t getAddressStringTable(uint32_t positionHeaderSection, uint32_t tailleHeaderSection, uint32_t positionStringTable, FILE* file) {
	const int currentPos = ftell(file);

	fseek(file, positionHeaderSection + (tailleHeaderSection * positionStringTable), SEEK_SET);

	ElfSecHeader* elfSecHeader2 = malloc(sizeof(ElfSecHeader));

  freadElfSecHEader(elfSecHeader2, sizeof(*elfSecHeader2), 1, file);

	fseek(file, currentPos, SEEK_SET);

	return reverseEndian32(elfSecHeader2->offset);
}

void freadElfHEader(ElfHeader* elfHeader, size_t size, size_t nmemb, FILE* file){
  size_t codeRet = fread(elfHeader, size, nmemb, file);
  gestionErr(codeRet, nmemb, file);
}

void freadElfSecHEader(ElfSecHeader* elfSecHeader, size_t size, size_t nmemb, FILE* file){
  size_t codeRet = fread(elfSecHeader, size, nmemb, file);
  gestionErr(codeRet, nmemb, file);
}

void freadData(uint32_t* data, size_t size, size_t nmemb, FILE* file){
  size_t codeRet = fread(data, size, nmemb, file);
  gestionErr(codeRet, nmemb, file);
}

void freadChar(char* name, size_t size, size_t nmemb, FILE* file){
  size_t codeRet = fread(name, size, nmemb, file);
  gestionErr(codeRet, nmemb, file);
}

void freadRealocationTable(RealocationEntry** realocationTable, size_t size, size_t nmemb, FILE* file){
  size_t codeRet = fread(*realocationTable, size, nmemb, file);
  gestionErr(codeRet, nmemb, file);
}


void gestionErr(size_t codeRet, size_t nmemb, FILE* file){
  if(codeRet != nmemb) {
      if (feof(file)){
        printf("Erreur de lecture du fichier: fin de fichier inattendue\n");
        exit(EXIT_FAILURE);
      } else if (ferror(file)) {
        perror("Erreur de lecture du fichier");
        exit(EXIT_FAILURE);
      }
  }
}


FILE *fopenR(const char *fichier, const char *mode){
  FILE* file = fopen(fichier, mode);
  if (file == NULL) {
    fprintf(stderr, "Value of errno: %d\n", errno);
    fprintf(stderr, "Error opening the file: %s\n", strerror( errno ));
    perror("Error printed by perror");
    exit(EXIT_FAILURE);
  }
  return file;
}
