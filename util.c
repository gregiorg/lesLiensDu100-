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

int isBigEndian() {
    static uint32_t one = 1;
    return ((* (uint8_t *) &one) == 0);
}

uint32_t reverseEndian32(uint32_t val) {
    uint8_t* tabIn = (uint8_t*) &val;
    uint8_t tabOut[sizeof (uint32_t)];

    for (int i = 0; i < sizeof (uint32_t); i++)
        tabOut[i] = tabIn[sizeof (uint32_t) - 1 - i];

    return *((uint32_t*) tabOut);
}

uint16_t reverseEndian16(uint16_t val) {
    uint8_t* tabIn = (uint8_t*) &val;
    uint8_t tabOut[sizeof (uint16_t)];

    for (int i = 0; i < sizeof (uint16_t); i++)
        tabOut[i] = tabIn[sizeof (uint16_t) - 1 - i];

    return *((uint16_t*) tabOut);
}

ElfHeaderF* getElfHeader(FILE* file) {
    ElfHeaderF* elfHeaderF = malloc(sizeof(ElfHeaderF));
    long int filePos = ftell(file);
    fseek(file, 0, SEEK_SET);

    ElfHeader elfHeader;
    if (fread(&elfHeader, sizeof (elfHeader), 1, file)){
      //cool
    }

    switch (elfHeader.indentClass) {
        case 0x0:
            elfHeaderF->indentClass = "invalide";
            break;
        case 0x1:
            elfHeaderF->indentClass = "32 bits";
            break;
        case 0x2:
            elfHeaderF->indentClass = "64 bits";
            break;
        default:
            elfHeaderF->indentClass = "inconnu";
    }

    switch (elfHeader.indentData) {
        case 0x0:
            elfHeaderF->indentData = "invalide";
            break;
        case 0x1:
            elfHeaderF->indentData = "petits";
            break;
        case 0x2:
            elfHeaderF->indentData = "gros";
            break;
        default:
            elfHeaderF->indentData = "inonnu";
    }

    switch (reverseEndian16(elfHeader.type)) {
        case 0x0:
            elfHeaderF->type = "aucun";
            break;
        case 0x1:
            elfHeaderF->type = "relogeable";
            break;
        case 0x2:
            elfHeaderF->type = "executable";
            break;
        default:
            elfHeaderF->type = "inonnu";
    }

    switch (reverseEndian16(elfHeader.machine)) {
        case 0x0:
            elfHeaderF->machine = "aucune";
            break;
        case 0x28:
            elfHeaderF->machine = "ARM";
            break;
        default:
            elfHeaderF->machine = "inconnu";
    }

    elfHeaderF->shoff = reverseEndian32(elfHeader.shoff);
    elfHeaderF->shnum = reverseEndian16(elfHeader.shnum);
    elfHeaderF->shentsize = reverseEndian16(elfHeader.shentsize);
    elfHeaderF->shsize = elfHeaderF->shnum * elfHeaderF->shentsize;
    elfHeaderF->shstrndx = reverseEndian16(elfHeader.shstrndx);

    elfHeaderF->ehsize = reverseEndian16(elfHeader.ehsize);

    fseek(file, filePos, SEEK_SET);
    return elfHeaderF;
}
