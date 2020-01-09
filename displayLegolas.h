#ifndef __DISPLAYLEGOLAS_H__
#define __DISPLAYLEGOLAS_H__

#include <stdlib.h>
#include <stdio.h>
#include "legolas.h"

void displayElfHeader(Header* header);
void displayElfSectionHeaderTable(Header* header);
void displaySectionsRawData(Header* header, char* sectionName);

#endif
