#ifndef __HEADER_CLASS_H__
#define __HEADER_CLASS_H__

#include <stdio.h>
#include "util.h"

extern int programsEndian;

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
    uint32_t offset;
    char type;
} RelocationTableEntry;

struct SectionHeader {
	char* name;
	uint32_t type;
	uint32_t flags;
	uint32_t addr;
	uint32_t link;
	uint32_t info;
	uint32_t addrAlign;
	uint32_t size;
    uint32_t entSize;
    uint32_t nbEntry;
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
    uint8_t indentOSABI;
    uint8_t indentABIVersion;
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint32_t entry;
    uint32_t flags;
    int shnum;
    SectionHeader** sectionHeaderTable;
} Header;

Header* legolasReadFromFile(FILE* file);
void legolasWriteToFile(Header* header, FILE* file);

void headerAddSection(Header*, SectionHeader*);
int headerGetIndexOfSectionHeader(Header*, SectionHeader*);
void headerRemoveSectionHeader(Header*, SectionHeader*);

void typeFirstRawDataPartIfNeeded(SectionHeader*, Header*);
void typeLastRawDataPartIfNeeded(SectionHeader*, Header*);
SymboleTableEntry* getSymboleTableEntryAddress(Header*, uint32_t);
char* getSymbolTableEntryName(Header* header, uint32_t);
SectionHeader* getSectionHeaderAddress(Header* header, uint16_t shndx);

char* sectionHeaderGetData(SectionHeader*);
uint32_t sectionHeaderGetEntSize(SectionHeader*);

void changeSymbolTableEntryPointerOnSectionHeaderOnFusion(Header*, SectionHeader*, SectionHeader*);
void updateRelocationTableEntryOffsetOnSectionHeaderOnFusion(Header*, SectionHeader*, SectionHeader*);
void symbolTableAddLocalEntry(SectionHeader*, SymboleTableEntry*, unsigned int);
void symbolTableAddGlobalEntry(SectionHeader*, SymboleTableEntry*, unsigned int);
void symboleTableRemoveEntry(SectionHeader*, SymboleTableEntry*);
void relocationTableAddEntry(SectionHeader*, RelocationTableEntry*);
void stringTableAddString(SectionHeader*, char*);
int stringTableGetIndex(SectionHeader*, char*);
char* stringTableGetString(SectionHeader*, int);

Elf32_Sym* sectionHeaderGetSymbolData(Header*, SectionHeader*, SectionHeader*);
Elf32_Rel* sectionHeaderGetUnexplicitRelocationData(SectionHeader*, SectionHeader*);
Elf32_Rela* sectionHeaderGetExplicitRelocationData(SectionHeader*);

/*
void typeRawDataIfNeeded(SectionHeader*, Header*);
SymboleTableEntry* getSymboleTableEntryAddress(Header*, uint32_t);
char* getSymbolTableEntryName(Header* header, uint32_t);
SectionHeader* getSectionHeaderAddress(Header* header, uint16_t shndx);
*/

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
