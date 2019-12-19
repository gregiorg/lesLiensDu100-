#ifndef __ETAPE1_H__
#define __ETAPE1_H__

#include <stdio.h>
#include <stdlib.h>
#include "util.h"

ElfHeaderF* getElfHeader(FILE* file); // Récupération de l'en-tête d'un fichier ELF déjà ouvert
void afficherHeader(ElfHeaderF* elfHeaderF); // Affichage de l'en-tête passé en paramètre

#endif
