#ifndef __ENUMS_H__
#define __ENUMS_H__

//TYPE DE SECTION
typedef enum = {
  SHT_NULL = 0,
  SHT_PROGBITS = 1,
  SHT_SYMTAB = 2,
  SHT_STRTAB = 3,
  SHT_RELA = 4,
  SHT_HASH = 5,
  SHT_DYNAMIC = 6,
  SHT_NOTE = 7,
  SHT_NOBITS = 8,
  SHT_REL = 9,
  SHT_SHLIB = 10,
  SHT_DYNSYM = 11,
  SHT_LOPROC = 0x70000000,
  SHT_ARM_ATTRIBUTES = 0x70000003,
  SHT_HIPROC = 0x7fffffff,
  SHT_LOUSER = 0x80000000,
  SHT_HIUSER = 0xffffffff
} SectionTypes;

//TYPE DE REIMPLANTATION
//If needed, add all the others relocation codes located in the ARM doc page 26 to 30
typedef enum {
	R_ARM_ABS32 = 2
} RelocationCodes;

//TYPE D'ARCHITECTURE
typedef enum = {
  INVALIDE_ARCH = 0x0,
  32_BITS = 0X1,
  64_BITS = 0x2
} ArchitectureTypes;

//TYPE DE BOUTISME
typedef enum {
  INVALIDE_ENDIAN = 0x0,
  LITTLE_ENDIAN = 0x1,
  BIG_ENDIAN = 0x2
}Endianness;

//TYPE DE FICHIER
typedef enum {
  NONE = 0x0,
  RELOCATABLE = 0x1,
  EXECUTABLE = 0x2
}FileTypes;

//TYPE DE MACHINE
typedef enum{
  MACHINE_NONE = 0x0,
  MACHINE_ARM = 0x28
}
