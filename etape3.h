#ifndef ETAPE3_H
#define ETAPE3_H

#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "etape2.c"

uint32_t* read_data_numsec(char* fname, int nsection);
uint32_t* read_data_nomsec(char* fname, char* nsection);

#endif
