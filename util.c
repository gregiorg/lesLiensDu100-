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

