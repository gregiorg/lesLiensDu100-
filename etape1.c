#include <stdio.h>

#include "util.h"

int main(int argc, char** argv) {
    FILE* file = fopen(argv[1], "r");

    ElfHeaderF* elf_header_f = get_elf_header(file);

    printf("Taille des mots : ");
    printf("%s\n", elf_header_f->indent_class);

    printf("Taille des indiens : ");
    printf("%s\n", elf_header_f->indent_data);

    printf("Type de fichier ELF : ");
    printf("%s\n", elf_header_f->type);

    printf("Platforme cible (architecture systeme) : ");
    printf("%s\n", elf_header_f->machine);

    printf("Table des sections : \n");

    printf("\tPosition : %d octets\n", elf_header_f->shoff);
    printf("\tNombre d'entrees : %d\n", elf_header_f->shnum);
    printf("\tTaille totale : %d octets\n", elf_header_f->shsize);
    printf("\tIndex de la table des noms de sections : %d\n", elf_header_f->shstrndx);

    printf("Taille totale de l'entete : %d octets\n", elf_header_f->ehsize);

    return 0;
}
