/*
ELF Loader - chargeur/implanteur d'exécutables au format ELF à but pédagogique
Copyright (C) 2012 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/

#include "util.h"
#include <stdint.h>
#include <stdlib.h>

int is_big_endian() {
    static uint32_t one = 1;
    return ((* (uint8_t *) &one) == 0);
}

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

ElfHeaderF* get_elf_header(FILE* file) {
    ElfHeaderF* elf_header_f = malloc(sizeof(ElfHeaderF));
    long int file_pos = ftell(file);
    fseek(file, 0, SEEK_SET);

    ElfHeader elf_header;
    fread(&elf_header, sizeof (elf_header), 1, file);

    switch (elf_header.indent_class) {
        case 0x0:
            elf_header_f->indent_class = "invalide";
            break;
        case 0x1:
            elf_header_f->indent_class = "32 bits";
            break;
        case 0x2:
            elf_header_f->indent_class = "64 bits";
            break;
        default:
            elf_header_f->indent_class = "inconnu";
    }

    switch (elf_header.indent_data) {
        case 0x0:
            elf_header_f->indent_data = "invalide";
            break;
        case 0x1:
            elf_header_f->indent_data = "petits";
            break;
        case 0x2:
            elf_header_f->indent_data = "gros";
            break;
        default:
            elf_header_f->indent_data = "inonnu";
    }

    switch (reverse_endian_16(elf_header.type)) {
        case 0x0:
            elf_header_f->type = "aucun";
            break;
        case 0x1:
            elf_header_f->type = "relogeable";
            break;
        case 0x2:
            elf_header_f->type = "executable";
            break;
        default:
            elf_header_f->type = "inonnu";
    }

    switch (reverse_endian_16(elf_header.machine)) {
        case 0x0:
            elf_header_f->machine = "aucune";
            break;
        case 0x28:
            elf_header_f->machine = "ARM";
            break;
        default:
            elf_header_f->machine = "inconnu";
    }

    elf_header_f->shoff = reverse_endian_32(elf_header.shoff);
    elf_header_f->shnum = reverse_endian_16(elf_header.shnum);
    elf_header_f->shentsize = reverse_endian_16(elf_header.shentsize);
    elf_header_f->shsize = elf_header_f->shnum * elf_header_f->shentsize;
    elf_header_f->shstrndx = reverse_endian_16(elf_header.shstrndx);

    elf_header_f->ehsize = reverse_endian_16(elf_header.ehsize);

    fseek(file, file_pos, SEEK_SET);
    return elf_header_f;
}
