#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t indent_magic_number[4];
    uint8_t indent_class;
    uint8_t indent_data;
    uint8_t indent_version;
    uint8_t indent_padding[9];
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint32_t entry;
    uint32_t phoff;
    uint32_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
} ElfHeader;

uint32_t reverse_endian_32(uint32_t val) {
    uint8_t* tab_in = (uint8_t*) &val;
    uint8_t tab_out[sizeof (uint32_t)];

    for (int i = 0; i < sizeof (uint32_t); i++)
        tab_out[i] = tab_in[sizeof (uint32_t) - 1 - i];

    return *((uint32_t*) tab_out);
}

uint16_t reverse_endian_16(uint16_t val) {
    uint8_t* tab_in = (uint8_t*) &val;
    uint8_t tab_out[sizeof (uint16_t)];

    for (int i = 0; i < sizeof (uint16_t); i++)
        tab_out[i] = tab_in[sizeof (uint16_t) - 1 - i];

    return *((uint16_t*) tab_out);
}

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

    printf("\n");

    return 0;
}
