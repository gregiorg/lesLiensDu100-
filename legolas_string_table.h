#ifndef __LEGOLAS_STRING_TABLE_H__
#define __LEGOLAS_STRING_TABLE_H__

#include "legolas.h"

void stringTableAddString(SectionHeader*, char*);
int stringTableGetIndex(SectionHeader*, char*);
char* stringTableGetString(SectionHeader*, int);

#endif
