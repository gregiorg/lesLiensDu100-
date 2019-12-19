#ifndef __ETAPE5_H__
#define __ETAPE5_H__

#include "etape4.h"

RealocationEntryF** getRealocationTable(FILE* file);
void afficherRealocationTab(RealocationEntryF** realocationTableF, int nbrEntry);

#endif
