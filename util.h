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

#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enums.h"

#define ELF32_R_SYM(info)    ((info)>>8)
#define ELF32_R_TYPE(info)   ((unsigned char)(info))

#define SH_REL 9

typedef struct {
    uint32_t offset;
    uint32_t info;
} RealocationEntry;

typedef struct {
    uint32_t offset;
    char SYM;
    char TYPE;
} RealocationEntryF;

typedef struct {
    uint8_t indentMagicNumber[4];
    uint8_t indentClass;
    uint8_t indentData;
    uint8_t indentVersion;
    uint8_t indentPadding[9];
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint32_t entry;
    uint32_t phoff;
    uint32_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
} ElfHeader;

typedef struct {
    char* indentClass;
    char* indentData;
    char* type;
    char* machine;
    uint32_t shoff;
    uint16_t shnum;
    uint16_t shstrndx;
    uint16_t shentsize;
    uint16_t shsize;
    uint16_t ehsize;
} ElfHeaderF;

typedef struct {
	uint32_t name;
	uint32_t type;
	uint32_t flags;
	uint32_t addr;
	uint32_t offset;
	uint32_t size;
	uint32_t link;
	uint32_t info;
	uint32_t addrAlign;
	uint32_t entSize;
} ElfSecHeader;

typedef struct {
	char * nameStr;
	char * typeStr;
	unsigned int indexTable;
	uint32_t indexName;
	uint32_t typeInt;
	uint32_t flags;
	uint32_t addr;
	uint32_t offset;
	uint32_t size;
	uint32_t link;
	uint32_t info;
	uint32_t addrAlign;
	uint32_t entSize;
} ElfSecHeaderF;

typedef struct {
  uint32_t    stName;   // Elf32_Word
  uint32_t    stValue;  // Elf32_Addr
  uint32_t    stSize;   // Elf32_Word
  unsigned char stInfo;
  unsigned char stOther;
  uint16_t    stShndx;  // Elf32_Half
} Elf32Sym;

#define ELF32_ST_BIND(i)   ((i)>>4)
#define ELF32_ST_TYPE(i)   ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

int is_big_endian();

uint32_t getAddressStringTable(uint32_t tailleHeaderSection, uint32_t positionStringTable, FILE* f);
uint32_t reverseEndian32(uint32_t val);
uint16_t reverseEndian16(uint16_t val);

char* getRelocationName(uint32_t relocationCode);
char* showType(uint32_t type);
char* showName(uint32_t indexName, uint32_t stringTableAddress, FILE* f);

#define reverse2(x) ((((x)&0xFF)<<8)|(((x)>>8)&0xFF))
#define reverse4(x) ((((x)&0xFF)<<24)|((((x)>>8)&0xFF)<<16)|((((x)>>16)&0xFF)<<8)|(((x)>>24)&0xFF))

#define min(x,y) ((x)<(y)?(x):(y))

#endif
