#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t indent[16];
    uint16_t type;
    uint16_t machine;
    uint16_t version;
    uint16_t entry;
    uint16_t phoff;
    uint16_t shoff;
    uint16_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
} ElfHeader;

uint16_t reverse_endian_16(uint16_t val) {
    uint8_t* tab_in = (uint8_t*) &val;
    uint8_t tab_out[2];

    for (int i = 0; i < 2; i++)
        tab_out[i] = tab_in[1-i];

    return *((uint16_t*) tab_out);
}

int main(int arcg, char** argv) {
    FILE* file = fopen(argv[1], "r");

    ElfHeader elf_header;
    fread(&elf_header, sizeof (elf_header), 1, file);

    printf("Mot magique : ");
    for (int i = 0; i < 16; i++) printf("%X ", elf_header.indent[i]);

    printf("\nType : ");

    switch (reverse_endian_16(elf_header.type)) {
        case 0:
            printf("aucun");
            break;
        case 1:
            printf("relogeable");
            break;
        case 2:
            printf("executable");
            break;
        default:
            printf("inconnu");
    }

    printf("\n");

    return 0;
}
