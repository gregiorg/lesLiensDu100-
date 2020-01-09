#include "legolas.h"

void relocationTableAddEntry(SectionHeader* sectionHeader, RelocationTableEntry* relocationEntry) {
   	//On vérifie que les section header et symbol table entry courantes ne soient pas NULL
   
   	if (sectionHeader == NULL) {
		printf("The current section header is NULL. The link edition will stop.");
		return;
    }
    if (relocationEntry == NULL) {
     	printf("The current relocation entry is NULL. The link edition will stop.");
		return;
    }

    sectionHeader->data.relocationTable = realloc(sectionHeader->data.relocationTable, sizeof(RelocationTableEntry*) * (sectionHeader->nbEntry+1));
    sectionHeader->data.relocationTable[sectionHeader->nbEntry] = relocationEntry;
    sectionHeader->nbEntry++;
    sectionHeader->size += sizeof(Elf32_Rel);
}

Elf32_Rel* sectionHeaderGetUnexplicitRelocationData(SectionHeader* sectionHeader, SectionHeader* symbolTableSectionHeader) {
	//A implémenter

	sectionHeader->size = sizeof(Elf32_Rel) * sectionHeader->nbEntry;
    Elf32_Rel* fileRelocationTable = malloc(sectionHeader->size);

    for (int i = 0; i < sectionHeader->nbEntry; i++) {
        RelocationTableEntry* relocationEntry = sectionHeader->data.relocationTable[i];
        Elf32_Rel* fileRelocationEntry = &(fileRelocationTable[i]);

        fileRelocationEntry->r_offset = reverseEndian32(relocationEntry->offset);
        char type = relocationEntry->type;
        uint32_t sym = symbolTableGetEntryIndex(symbolTableSectionHeader, relocationEntry->sym);
        fileRelocationEntry->r_info = reverseEndian32(ELF32_R_INFO(sym, type));
    }

    return fileRelocationTable;

	//return NULL;
}

/*
Elf32_Rela* sectionHeaderGetExplicitRelocatonData(SectionHeader* sectionHeader) {
}
*/

void updateRelocationTableEntryOffsetOnSectionHeaderOnFusion(Header* header, SectionHeader* sectionHeaderFile2, SectionHeader* sectionHeaderFile1) {
     /*
        Après fusion des sections PROGBITS, on doit faire pointer les symbol table entry
        vers les nouvelles sections créées par la fusion (actuellement, elles pointent sur
        des sections qui "n'existent plus").
     */

     for (int k=0; k < header->shnum; k++) {
          SectionHeader* symboleTableHeader = header->sectionHeaderTable[k];

          if (symboleTableHeader->type == SHT_REL) {
               for (int l=0; l < symboleTableHeader->nbEntry; l++) {
                    if (symboleTableHeader->data.relocationTable[l]->sym->sectionHeader == sectionHeaderFile2) {
                         symboleTableHeader->data.relocationTable[l]->offset += sectionHeaderFile1->size;
                    }
               }
               break;
          }
     }
}
