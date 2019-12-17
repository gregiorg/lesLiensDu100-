#ifndef ETAPE2_H
#define ETAPE2_H

#include <stdlib.h>
#include "util.h"
#include <string.h>

void putCurrentHeader(ElfSecHeader elfSecHeader, uint32_t stringTableAddress, FILE* f, int i, ElfSecHeaderF* elfSecHeaderF);

char* showType(uint32_t type);

char* showName(uint32_t name, uint32_t stringTableAddress, FILE* f);

uint32_t getAddressStringTable(uint32_t tailleHeaderSection, uint32_t positionStringTable, FILE* f);

void afficheFinal(ElfSecHeaderF* finalHeader, uint32_t nbSections);

ElfSecHeaderF** etape2(char* fname);

#endif
