#ifndef __LEGOLAS_RELOCATION_TABLE_H__
#define __LEGOLAS_RELOCATION_TABLE_H__

#include "legolas.h"

struct RelocationTableEntry {
    SymboleTableEntry* sym;
    uint32_t offset;
    char type;
};

void updateRelocationTableEntryOffsetOnSectionHeaderOnFusion(Header*, SectionHeader*, SectionHeader*);
void relocationTableAddEntry(SectionHeader*, RelocationTableEntry*);
Elf32_Rel* sectionHeaderGetUnexplicitRelocationData(SectionHeader*, SectionHeader*);
Elf32_Rela* sectionHeaderGetExplicitRelocationData(SectionHeader*);

#endif
