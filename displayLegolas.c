#include "displayLegolas.h"

void displayElfHeader(Header* header) {

  char* archi = malloc(30);
  char* boutisme = malloc(30);
  char* fichier = malloc(30);
  char* machine = malloc(30);

  //Architecture
      switch (header->indentClass) {
          case INVALIDE_ARCH:
              archi = "invalide";
              break;
          case BITS_32:
              archi = "32 bits";
              break;
          case BITS_64:
              archi = "64 bits";
              break;
          default:
              archi = "inconnu";
      }

  //type de boutisme
      switch (header->indentData) {
          case INVALIDE_END:
              boutisme = "invalide";
              break;
          case LITTLE_END:
              boutisme = "petits";
              break;
          case BIG_END:
              boutisme = "gros";
              break;
          default:
              boutisme = "inconnu";
      }

  //Type de fichier
      switch (header->type) {
          case NONE:
              fichier = "aucun";
              break;
          case REALOCATABLE:
              fichier = "relogeable";
              break;
          case EXECUTABLE:
              fichier = "executable";
              break;
          default:
              fichier = "inconnu";
      }

      //Type de machine
          switch (header->machine) {
              case MACHINE_NONE:
                  machine = "none";
                  break;
              case MACHINE_ARM:
                  machine = "ARM";
                  break;
              default:
                  machine = "inconnu";
          }

  printf("Taille des mots : %s\n", archi);
  printf("Taille des indiens : %s\n", boutisme);
  printf("Type de fichier ELF : %s\n", fichier);
  printf("Platforme cible (architecture systeme) : %s\n", machine);
  printf("Nombre d'entrees dans la table des sections : %d\n", header->shnum);
}

void displayElfSectionHeaderTable(Header* header) {

  for(int i = 0; i < header->shnum; i++) {
    printf("Table numéro : %d\n", i);
    printf("  Nom de la section : %s\n", header->sectionHeaderTable[i]->name);
    printf("  Type de la section : %s\n", showType(header->sectionHeaderTable[i]->type));
    printf("  Flags : 0x%x\n", header->sectionHeaderTable[i]->flags);
    printf("	Adresse : 0x%x\n", header->sectionHeaderTable[i]->addr);
    printf("	Link : 0x%x\n", header->sectionHeaderTable[i]->link);
    printf("	Info : 0x%x\n", header->sectionHeaderTable[i]->info);
    printf("	Adresse alignement : 0x%x\n", header->sectionHeaderTable[i]->addrAlign);
    printf("	Taille : 0x%x\n", header->sectionHeaderTable[i]->size);
    printf("	Taille d'une entité : 0x%x\n", header->sectionHeaderTable[i]->entSize);
    printf("  Nombre d'entités : 0x%x\n", header->sectionHeaderTable[i]->nbEntry);
  }
}

void displaySectionsRawData(Header* header, char* sectionName) {

  int sectionIndex = 0;
  while(sectionIndex < header->shnum && strcpy(header->sectionHeaderTable[sectionIndex]->name, ".strtab") != 0) {
    sectionIndex++;
  }

  //TODO extract data and display
}
