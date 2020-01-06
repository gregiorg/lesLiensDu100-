#ifndef __ETAPE1_H__
#define __ETAPE1_H__

#include "util.h"

ElfHeaderF* getElfHeader(FILE*); // Récupération de l'en-tête d'un fichier ELF déjà ouvert
void afficherHeader(ElfHeaderF*); // Affichage de l'en-tête passé en paramètre

#endif
