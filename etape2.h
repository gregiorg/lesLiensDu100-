#ifndef ETAPE2_H
#define ETAPE2_H

#include <stdlib.h>
#include "util.h"
#include <string.h>

void putCurrentHeader(ElfSecHeader elfSecHeader, uint32_t stringTableAddress, FILE* f, int i, ElfSecHeaderF** elfSecHeaderF);

void afficheFinal(ElfSecHeaderF** finalHeader, uint32_t nbSections);

ElfSecHeaderF** etape2(FILE* f);

#endif
