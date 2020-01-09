#ifndef __LEGOLAS_H__
#define __LEGOLAS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

extern int programsEndian;

typedef struct Header Header;
typedef struct SectionHeader SectionHeader;
typedef struct SymboleTableEntry SymboleTableEntry;
typedef struct RelocationTableEntry RelocationTableEntry;

struct Header {
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
};

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

struct SymboleTableEntry {
    char* name;
    uint32_t value;
    uint32_t size;
    char bind;
    char type;
    char other;
    SectionHeader* sectionHeader;
};

struct RelocationTableEntry {
    SymboleTableEntry* sym;
    uint32_t offset;
    char type;
};

Header* legolasReadFromFile(FILE* file);
void legolasWriteToFile(Header* header, FILE* file);

void headerAddSection(Header*, SectionHeader*);
void headerRemoveSectionHeader(Header*, SectionHeader*);
int headerGetIndexOfSectionHeader(Header*, SectionHeader*);
SectionHeader* getSectionHeaderFromName(Header*, char*);
SectionHeader** getSectionHeadersFromType(Header*, uint32_t, size_t*);
void setHeader(Elf32_Ehdr*, Header*);

void typeFirstRawDataPartIfNeeded(SectionHeader*, Header*);
void typeLastRawDataPartIfNeeded(SectionHeader*, Header*);
SectionHeader* getSectionHeaderAddress(Header* header, uint16_t shndx);
char* sectionHeaderGetData(SectionHeader*);
uint32_t sectionHeaderGetEntSize(SectionHeader*);
void setSectionHeader(Header*, Elf32_Shdr*, int, FILE*);
char* sectionHeaderGetRawData(SectionHeader*);

void stringTableAddString(SectionHeader*, char*);
int stringTableGetIndex(SectionHeader*, char*);
char* stringTableGetString(SectionHeader*, int);
SectionHeader* stringTableCreate(char*);

SymboleTableEntry* getSymboleTableEntryAddress(Header*, uint32_t);
char* getSymbolTableEntryName(Header* header, uint32_t);
void changeSymbolTableEntryPointerOnSectionHeaderOnFusion(Header*, SectionHeader*, SectionHeader*);
void symbolTableAddGlobalEntry(SectionHeader*, SymboleTableEntry*, unsigned int);
void symbolTableAddLocalEntry(SectionHeader*, SymboleTableEntry*, unsigned int);
void symboleTableRemoveEntry(SectionHeader*, SymboleTableEntry*);
Elf32_Sym* sectionHeaderGetSymbolData(Header*, SectionHeader*, SectionHeader*);
int symbolTableGetEntryIndex(SectionHeader*, SymboleTableEntry*);

void updateRelocationTableEntryOffsetOnSectionHeaderOnFusion(Header*, SectionHeader*, SectionHeader*);
void relocationTableAddEntry(SectionHeader*, RelocationTableEntry*);
Elf32_Rel* sectionHeaderGetUnexplicitRelocationData(SectionHeader*, SectionHeader*);
Elf32_Rela* sectionHeaderGetExplicitRelocationData(SectionHeader*);

#endif
