#ifndef __LEGOLAS_SYMBOL_TABLE_H__
#define __LEGOLAS_SYMBOL_TABLE_H__

#include "legolas.h"

struct {
    char* name;
    uint32_t value;
    uint32_t size;
    char bind;
    char type;
    char other;
    SectionHeader* sectionHeader;
} SymboleTableEntry;

SymboleTableEntry* getSymboleTableEntryAddress(Header*, uint32_t);
char* getSymbolTableEntryName(Header* header, uint32_t);
void changeSymbolTableEntryPointerOnSectionHeaderOnFusion(Header*, SectionHeader*, SectionHeader*);
void symbolTableAddGlobalEntry(SectionHeader*, SymboleTableEntry*, unsigned int);
void symboleTableRemoveEntry(SectionHeader*, SymboleTableEntry*);
Elf32_Sym* sectionHeaderGetSymbolData(Header*, SectionHeader*, SectionHeader*);

#endif
