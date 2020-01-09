#include "displayLegolas.h"

void displayElfHeader(Header* header) {

  // extracting informations as char*
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

  // iterating on all section headers
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

  // extracting a section header with a given name
  int sectionIndex = 0;
  while(sectionIndex < header->shnum && strcmp(header->sectionHeaderTable[sectionIndex]->name, sectionName) != 0) {
    sectionIndex++;
  }
  SectionHeader* currentSection = header->sectionHeaderTable[sectionIndex];

  // extracting and displaying raw data
  uint32_t* data = currentSection->rawData;
  for (int i = 0; i < currentSection->size/sizeof(uint32_t); i++) {
      printf("%08X\n", data[i]);
    }
}

void displaySymbolTable(Header* header) {

  // extracting the symbols table header
  int symbolTableIndex = 0;
  while(symbolTableIndex < header->shnum && strcmp(header->sectionHeaderTable[symbolTableIndex]->name, ".symtab") != 0) {
    symbolTableIndex++;
  }
  SectionHeader* symbolTableHeader = header->sectionHeaderTable[symbolTableIndex];

  // extracting symbol table entries
  SymboleTableEntry** symbolTable = symbolTableHeader->data.symboleTable;

  //displaying symbol table entries
  for(int i = 0; i < symbolTableHeader->nbEntry; i++) {
    printf("Symbole n°%d :\n", i);
    printf("	Nom du symbole : %s\n", symbolTable[i]->name);
    printf("	Valeur du symbole : %08X\n", symbolTable[i]->value);
    printf("	Taille du symbole : %08X\n", symbolTable[i]->size);
  }
}

void displayRealocationTable(Header* header) {

  // extracting the realocation table header
  int realocTableIndex = 0;
  while(realocTableIndex < header->shnum && strcmp(header->sectionHeaderTable[realocTableIndex]->name, ".symtab") != 0) {
    realocTableIndex++;
  }
  SectionHeader* realocTableHeader = header->sectionHeaderTable[realocTableIndex];

  // extracting realocation table entries
  RelocationTableEntry** realocTable = realocTableHeader->data.relocationTable;

  // displaying realocation entries
  for(int i = 0; i < realocTableHeader->nbEntry; i++) {
    printf("Entrée de la table de realocation n°%d\n", i);
    printf("\tOffset : %08X\n", realocTable[i]->offset);
    printf("\tAdresse du symbole : %p\n", realocTable[i]->sym);
    printf("\tType : %d\n", realocTable[i]->type);
  }
}
