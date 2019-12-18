#ifndef ETAPE2_H
#define ETAPE2_H

#include <string.h>
#include "etape1.h"

void putCurrentHeader(ElfSecHeader elfSecHeader, uint32_t stringTableAddress, FILE* f, int i, ElfSecHeaderF** elfSecHeaderF);

void afficheFinal(ElfSecHeaderF** finalHeader, uint32_t nbSections);

ElfSecHeaderF** etape2(FILE* f);

#endif
