#include <stdio.h>

#include "util.h"

int main(int argc, char** argv) {
    FILE* file = fopen(argv[1], "r");

    ElfHeaderF* elfHeaderF = getElfHeader(file);

    printf("Taille des mots : %s\n", elfHeaderF->indentClass);
    printf("Taille des indiens : %s\n", elfHeaderF->indentData);
    printf("Type de fichier ELF : %s\n", elfHeaderF->type);
    printf("Platforme cible (architecture systeme) : %s\n", elfHeaderF->machine);

    printf("Table des sections : \n");
    printf("\tPosition : %d octets\n", elfHeaderF->shoff);
    printf("\tNombre d'entrees : %d\n", elfHeaderF->shnum);
    printf("\tTaille totale : %d octets\n", elfHeaderF->shsize);
    printf("\tIndex de la table des noms de sections : %d\n", elfHeaderF->shstrndx);

    printf("Taille totale de l'entete : %d octets\n", elfHeaderF->ehsize);

    return 0;
}
