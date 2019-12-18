#ifndef __ETAPE4_H__
#define __ETAPE4_H__

#include "etape3.h"

void afficheTabSym(Elf32Sym** tabSym, int size, uint32_t stringTableAddress, FILE* f);
Elf32Sym** etape4(FILE* f);

#endif
