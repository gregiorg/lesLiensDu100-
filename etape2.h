#ifndef ETAPE2_H
#define ETAPE2_H

#include <stdlib.h>
#include "util.h"
#include <string.h>

void putCurrentHeader(Elf_SecHeader elf_secHeader, uint32_t stringTableAddress, FILE* f, int i, Elf_SecHeaderF * elf_secHeaderF);
char * showType(uint32_t type);
char * showName(uint32_t name, uint32_t stringTableAddress, FILE *f);
uint32_t getAddressStringTable(uint32_t tailleHeaderSection, uint32_t positionStringTable, FILE* f);
Elf_SecHeaderF returnFinalHeader(Elf_SecHeaderF elf_secHeaderF);
void afficheFinal(Elf_SecHeaderF * finalHeader, uint32_t nbSections);
Elf_SecHeaderF* etape2(char* fname);
#endif
