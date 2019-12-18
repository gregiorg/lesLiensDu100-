#ifndef __ETAPE6_H__
#define __ETAPE6_H__

#include "etape5.h"

int countNbSecProgbits(ElfSecHeaderF** allSecHeaders, int nbSecHeaders);
ElfSecHeaderF** allocSecHeader(int nbElem);
ElfSecHeaderF** extractProgbit(ElfSecHeaderF** allSecHeaders, int nbAllSecHeaders, int nbSecProgbit);

#endif
