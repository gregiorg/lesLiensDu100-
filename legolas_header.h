#ifndef __LEGOLAS_HEADER_H__
#define __LEGOLAS_HEADER_H__

#include "legolas.h"

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

void headerAddSection(Header*, SectionHeader*);
void headerRemoveSectionHeader(Header*, SectionHeader*);
int headerGetIndexOfSectionHeader(Header*, SectionHeader*);
SectionHeader* getSectionHeaderFromName(Header*, char*);
SectionHeader** getSectionHeadersFromType(Header*, uint32_t, size_t*);

#endif
