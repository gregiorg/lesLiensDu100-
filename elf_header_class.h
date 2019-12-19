#ifndef __HEADER_CLASS_H__
#define __HEADER_CLASS_H__

#include <stdio.h>
#include "util.h"

typedef struct SymboleTableEntry SymboleTableEntry;
typedef struct SectionHeader SectionHeader;

struct SymboleTableEntry {
    char* name;
    uint32_t value;
    uint32_t size;
    char bind;
    char type;
    char other;
    SectionHeader* sectionHeader;
};

typedef struct {
    SymboleTableEntry* sym;
    char type;
} RelocationTableEntry;


struct SectionHeader {
	unsigned int indexTable;
	char* name;
	uint32_t type;
	uint32_t flags;
	uint32_t addr;
	uint32_t link;
	uint32_t info;
	uint32_t addrAlign;
	uint32_t entSize;
	uint32_t size;
    void* rawData;
	union {
		char* stringTable;
		SymboleTableEntry** symboleTable;
		RelocationTableEntry** relocationTable;
	} data;
};

typedef struct {
    uint8_t indentClass;
    uint8_t indentData;
    uint8_t indentVersion;
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint32_t entry;
    uint32_t flags;
    int nbSections;
    SectionHeader** sections;
} Header;

Header* headerFromFile(FILE* file);
void headerWriteToFile(Header* header, FILE* file);
void typeRawDataIfNeeded(SectionHeader*, Header*);
SymboleTableEntry* getSymboleTableEntryAddress(Header*, uint32_t);
char* getSymbolTableEntryName(Header* header, uint32_t);
SectionHeader* getSectionHeaderAddress(Header* header, uint16_t shndx);

//void headerPrint(Header*);
//void headerWriteToFile(Header*, FILE* file);
//char* headerGetString(int index);

//int sectionHeaderTableGetSize(SectionHeaderTable*);
//SectionHeader** sectionHeaderTableGetSectionsOfType(SectionHeaderTable*, uint32_t, int*);
//SectionHeader** sectionHeaderTableGetSectionsWithName(SectionHeaderTable*, char*, int*);
//SectionHeader* sectionHeaderTableGetSectionOfType(SectionHeaderTable*, uint32_t);
//SectionHeader* sectionHeaderTableGetSectionWithName(SectionHeaderTable*, char*);

//const char* sectionHeaderGetNameString(SectionHeader*);
//const char* sectionHeaderGetTypeString(SectionHeader*);
//int sectionHeaderGetSize(SectionHeader*);
//RelocationTable* sectionHeaderGetRelocationTable(SectionHeader*);
//SymboleTable* sectionHeaderGetSymboleTable(SectionHeader*);
#endif
