#ifndef __LEGOLAS_H__
#define __LEGOLAS_H__

#include <stdio.h>
#include "util.h"

typedef struct Header Header;
typedef struct SectionHeader SectionHeader;
typedef struct SymboleTableEntry SymboleTableEntry;
typedef struct RelocationTableEntry RelocationTableEntry;

#include "legolas_header.h"
#include "legolas_section.h"
#include "legolas_string_table.h"
#include "legolas_symbol_table.h"
#include "legolas_relocation_table.h"

extern int programsEndian;

Header* legolasReadFromFile(FILE* file);
void legolasWriteToFile(Header* header, FILE* file);

#endif
