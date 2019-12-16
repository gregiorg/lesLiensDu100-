#include <stdio.h>

#include "util.h"

int main(int argc, char** argv) {
    FILE* file = fopen(argv[1], "r");

    ElfHeader elf_header;
    fread(&elf_header, sizeof (elf_header), 1, file);

    printf("Taille des mots : ");
    switch (elf_header.indent_class) {
        case 0x0:
            printf("invalide");
            break;
        case 0x1:
            printf("32 bits");
            break;
        case 0x2:
            printf("64 bits");
            break;
        default:
            printf("inconnu");
    }

    printf("\nTaille des indiens : ");
    switch (elf_header.indent_data) {
        case 0x0:
            printf("invalide");
            break;
        case 0x1:
            printf("petits");
            break;
        case 0x2:
            printf("gros");
            break;
        default:
            printf("inconnu");
    }

    printf("\nType de fichier ELF : ");
    switch (reverse_endian_16(elf_header.type)) {
        case 0x0:
            printf("aucun");
            break;
        case 0x1:
            printf("relogeable");
            break;
        case 0x2:
            printf("executable");
            break;
        default:
            printf("inconnu");
    }

    printf("\nPlatforme cible (architecture systeme) : ");
    switch (reverse_endian_16(elf_header.machine)) {
        case 0x0:
            printf("aucun");
            break;
        case 0x28:
            printf("ARM");
            break;
        default:
            printf("inconnu");
    }

    printf("\nTable des sections : ");
    printf("\n\tPosition : %d octets", reverse_endian_32(elf_header.shoff));
    printf("\n\tNombre d'entrees : %d", reverse_endian_16(elf_header.shnum));
    printf("\n\tTaille totale : %d octets", reverse_endian_16(elf_header.shentsize)*reverse_endian_16(elf_header.shnum));
    printf("\n\tIndex de la table des noms de sections : %d octets", reverse_endian_16(elf_header.shstrndx));

    printf("\nTaille totale de l'entete : %d octets", reverse_endian_16(elf_header.ehsize));

    printf("\n");

    return 0;
}
