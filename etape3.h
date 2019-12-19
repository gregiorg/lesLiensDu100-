#ifndef __ETAPE3_H__
#define __ETAPE3_H__

#include "etape2.h"

uint32_t* readDataNumSec(FILE* f, int numSection); // Récupération des données de la section de numéro numSection
uint32_t* readDataNomSec(FILE* f, char* nomSection); // Récupération des données de la section de nom nomSection
void afficherDataSection(uint32_t* data, ElfSecHeaderF** elfSecHeader, int numSection); // Affichage des données d'une section

#endif
