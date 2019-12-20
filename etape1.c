//------------------AFFICHAGE DE L'ENTETE D'UN FICHIER ELF-------------
#include "etape1.h"

//Calcul de l'entete d'un fichier  ELF
Elf32_Ehdr* getElfHeader(FILE* file) {
    long int filePos = ftell(file);
    fseek(file, 0, SEEK_SET);

    Elf32_Ehdr* elfHeader = malloc(sizeof(Elf32_Ehdr));
    size_t codeRet = fread(&elfHeader, sizeof (elfHeader), 1, file);
    if(codeRet != 1) {
        if (feof(file)){
          printf("Erreur de lecture du fichier: fin de fichier inattendue\n");
          exit(EXIT_FAILURE);
        } else if (ferror(file)) {
          perror("Erreur de lecture du fichier");
          exit(EXIT_FAILURE);
        }
    }

    elfHeader->e_type = reverseEndian16(elfHeader->e_type);
    elfHeader->e_machine = reverseEndian16(elfHeader->e_machine);
    elfHeader->e_version = reverseEndian32(elfHeader->e_version);
    elfHeader->e_entry = reverseEndian32(elfHeader->e_entry);
    elfHeader->e_phoff = reverseEndian32(elfHeader->e_phoff);
    elfHeader->e_shoff = reverseEndian32(elfHeader->e_shoff);
    elfHeader->e_flags = reverseEndian32(elfHeader->e_flags);
    elfHeader->e_ehsize = reverseEndian16(elfHeader->e_ehsize);
    elfHeader->e_phentsize = reverseEndian16(elfHeader->e_phentsize);
    elfHeader->e_phnum = reverseEndian16(elfHeader->e_phnum);
    elfHeader->e_shentsize = reverseEndian16(elfHeader->e_shentsize);
    elfHeader->e_shnum = reverseEndian16(elfHeader->e_shnum);
    elfHeader->e_shstrndx = reverseEndian16(elfHeader->e_shstrndx);

    fseek(file, filePos, SEEK_SET);
    return elfHeader;
}

//Fonction d'affichage
void afficherHeader(Elf32_Ehdr* elfHeader) {
  //------------MAGIC NUMBERS-----------------
  //Architecture
      switch (elfHeader.indentClass) {
          case INVALIDE_ARCH:
              elfHeaderF->indentClass = "invalide";
              break;
          case BITS_32:
              elfHeaderF->indentClass = "32 bits";
              break;
          case BITS_64:
              elfHeaderF->indentClass = "64 bits";
              break;
          default:
              elfHeaderF->indentClass = "inconnu";
      }

  //type de boutisme
      switch (elfHeader.indentData) {
          case INVALIDE_END:
              elfHeaderF->indentData = "invalide";
              break;
          case LITTLE_END:
              elfHeaderF->indentData = "petits";
              break;
          case BIG_END:
              elfHeaderF->indentData = "gros";
              break;
          default:
              elfHeaderF->indentData = "inconnu";
      }

  //Type de fichier
      switch (reverseEndian16(elfHeader.type)) {
          case NONE:
              elfHeaderF->type = "aucun";
              break;
          case REALOCATABLE:
              elfHeaderF->type = "relogeable";
              break;
          case EXECUTABLE:
              elfHeaderF->type = "executable";
              break;
          default:
              elfHeaderF->type = "inconnu";
      }

  //Type de machine
      switch (reverseEndian16(elfHeader.machine)) {
          case MACHINE_NONE:
              elfHeaderF->machine = "aucune";
              break;
          case MACHINE_ARM:
              elfHeaderF->machine = "ARM";
              break;
          default:
              elfHeaderF->machine = "inconnu";
      }
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
