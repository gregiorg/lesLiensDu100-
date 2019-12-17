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

typedef struct {
    uint8_t indent_magic_number[4];
    uint8_t indent_class;
    uint8_t indent_data;
    uint8_t indent_version;
    uint8_t indent_padding[9];
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
    char* indent_class;
    char* indent_data;
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
} Elf_SecHeader;

#define ELF32_ST_BIND(i)   ((i)>>4)
#define ELF32_ST_TYPE(i)   ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

typedef struct {
  uint32_t    st_name;   // Elf32_Word
  uint32_t    st_value;  // Elf32_Addr
  uint32_t    st_size;   // Elf32_Word
  unsigned char st_info;
  unsigned char st_other;
  uint16_t    st_shndx;  // Elf32_Half
} Elf32_Sym;

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
} Elf_SecHeaderF;

int is_big_endian();

uint32_t reverse_endian_32(uint32_t val);
uint16_t reverse_endian_16(uint16_t val);

ElfHeaderF* get_elf_header(FILE* file);

#define reverse_2(x) ((((x)&0xFF)<<8)|(((x)>>8)&0xFF))
#define reverse_4(x) ((((x)&0xFF)<<24)|((((x)>>8)&0xFF)<<16)|\
						((((x)>>16)&0xFF)<<8)|(((x)>>24)&0xFF))

#define min(x,y) ((x)<(y)?(x):(y))

#endif
