#ifndef ETAPE2_H
#define ETAPE2_H

#include "etape1.h"

void putCurrentHeader(ElfSecHeader elfSecHeader, uint32_t stringTableAddress, FILE* f, int i, ElfSecHeaderF** elfSecHeaderF); // Ajoute l'en-tête de section actuel au tableau des en-tête de section
void afficherTabSecHeader(ElfSecHeaderF** finalHeader, uint32_t nbSections);  // Affichage de la table des sections
ElfSecHeaderF** getTabElfSecHeader(FILE* f); // Récupération de la table des sections à partir d'un fichier ELF déjà ouvert

#endif
