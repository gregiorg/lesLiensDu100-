//---------------AFFICHAGE DES TABLES DE REIMPLANTATION-------------------------
#include "etape5.h"

RealocationEntryF** getRealocationTable(FILE* file) {
    long int filePos = ftell(file);

    ElfHeaderF* header = getElfHeader(file);  // get the files header
    ElfSecHeaderF** sectionTable = getTabElfSecHeader(file);  // get the files section tailleHeaderSection

    // compute the total number of rel entries
    int nbrRelEnt = 0;
    for (int i = 0; i < header->shnum; i++) {
      if (sectionTable[i]->typeInt == SHT_REL) {
        nbrRelEnt += sectionTable[i]->size / sectionTable[i]->entSize;
      }
    }

    // alocate memory space for all the rel entries
    RealocationEntryF** realocationTableF = malloc(sizeof(RealocationEntryF*) * nbrRelEnt);
    for (int i = 0; i < nbrRelEnt; i++) {
      realocationTableF[i] = malloc(sizeof(RealocationEntryF));
    }

    // extract rel entries
    int relIndex = 0;
    for (int i = 0; i < header->shnum; i++) {
      if (sectionTable[i]->typeInt == SHT_REL) {

        int nbrRelInSection = sectionTable[i]->size / sectionTable[i]->entSize;  // compute number of rel entries in the section

        // allocate memory space for the temporary rel entry table
        RealocationEntry** realocationTable = malloc(sizeof(RealocationEntry*) * nbrRelInSection);
        for (int i = 0; i < nbrRelInSection; i++) {
          realocationTable[i] = malloc(sizeof(RealocationEntry));
        }

        fseek(file, sectionTable[i]->offset, SEEK_SET);  // put the cursor to the current sections data

        // read the rel entries in the current section
        freadRealocationTable(realocationTable, sizeof(RealocationEntry), nbrRelInSection, file);

        // converting data
        for (int i = 0; i < nbrRelInSection; i++) {
            realocationTableF[i + relIndex]->offset = reverseEndian32(realocationTable[i]->offset);
            realocationTableF[i + relIndex]->SYM = ELF32_R_SYM(reverseEndian32(realocationTable[i]->info));
            realocationTableF[i + relIndex]->TYPE = ELF32_R_TYPE(reverseEndian32(realocationTable[i]->info));
        }

        // incrementing the total index
        relIndex += nbrRelInSection;
      }
    }

    fseek(file, filePos, SEEK_SET);
    return realocationTableF;
}

//Affichage de la table
void afficherRealocationTab(RealocationEntryF** realocationTableF, int nbrEntry) {
  for (int j = 0; j < nbrEntry; j++) {
    printf("Entrée de la table de realocation n°%d\n", j);
    printf("\tOffset : %08X\n", realocationTableF[j]->offset);
    printf("\tIndex du symbole : %02X\n", realocationTableF[j]->SYM);
    printf("\tType : %02X\n", realocationTableF[j]->TYPE);
    printf("\n");
  }
}
