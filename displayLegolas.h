#ifndef __DISPLAYLEGOLAS_H__
#define __DISPLAYLEGOLAS_H__

#include <stdlib.h>
#include <stdio.h>
#include "legolas.h"

// etape1
void displayElfHeader(Header* header);

// etape2
void displayElfSectionHeaderTable(Header* header);

// etape3
void displaySectionsRawData(Header* header, char* sectionName);

// etape4
void displaySymbolTable(Header* header);

// etape5
void displayRealocationTable(Header* header);

#endif
