#include "etape1.h"

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

void afficherHeader(ElfHeaderF* elfHeaderF) {
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
}
