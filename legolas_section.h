#ifndef __LEGOLAS_SECTION_H__
#define __LEGOLAS_SECTION_H__

#include "legolas.h"

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

void typeFirstRawDataPartIfNeeded(SectionHeader*, Header*);
void typeLastRawDataPartIfNeeded(SectionHeader*, Header*);
SectionHeader* getSectionHeaderAddress(Header* header, uint16_t shndx);
char* sectionHeaderGetData(SectionHeader*);
uint32_t sectionHeaderGetEntSize(SectionHeader*);

#endif
