#ifndef __ETAPE4_H__
#define __ETAPE4_H__

#include "etape3.h"

void afficherTabSym(Elf32Sym** tabSym, int size, uint32_t stringTableAddress, FILE* f); // Affichage de la table des symboles
Elf32Sym** getTabSym(FILE* f); // Récupération de la table des symboles d'un fichier ELF déjà ouvert

#endif
