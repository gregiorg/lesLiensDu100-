#ifndef __ETAPE1_H__
#define __ETAPE1_H__

#include <stdio.h>
#include <stdlib.h>
#include "util.h" 

ElfHeaderF* getElfHeader(FILE* file);
void afficherHeader(ElfHeaderF* elfHeaderF);

#endif
