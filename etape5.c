#include "etape5.h"

RealocationEntryF** getRealocationTable(FILE* file) {
    ElfHeaderF* header = getElfHeader(file);
    ElfSecHeaderF** sectionTable = getTabElfSecHeader(file);
    RealocationEntryF** realocationTableF = NULL;

    for (int i = 0; i < header->shnum; i++) {
        ElfSecHeaderF* sectionHeader = sectionTable[i];

        if (sectionHeader->typeInt == SH_REL) {
            printf("RealocationTable : %08X\n", sectionHeader->offset);

            int nbrEntry = sectionHeader->size / sectionHeader->entSize;

            RealocationEntry** realocationTable = malloc(sizeof(RealocationEntry*) * nbrEntry);
            for(int k = 0; k < nbrEntry; i++) {
              realocationTable[k] = malloc(sizeof(RealocationEntry));
            }

            fseek(file, sectionHeader->offset, SEEK_SET);
            size_t codeRet = fread(realocationTable, sizeof(RealocationEntry), nbrEntry, file);
            if(codeRet != 1) {
                if (feof(file)){
                  printf("Erreur de lecture du fichier: fin de fichier inattendue\n");
                  exit(EXIT_FAILURE);
                } else if (ferror(file)) {
                  perror("Erreur de lecture du fichier");
                  exit(EXIT_FAILURE);
                }
            }

            realocationTableF = malloc(sizeof(RealocationEntryF*) * nbrEntry);
            for(int l = 0; l < nbrEntry; i++) {
              realocationTableF[l] = malloc(sizeof(RealocationEntryF));
            }

            for (int j = 0; j < nbrEntry; j++) {
                realocationTableF[j]->offset = reverseEndian32(realocationTable[j]->offset);
                realocationTableF[j]->SYM = ELF32_R_SYM(reverseEndian32(realocationTable[j]->info));
                realocationTableF[j]->TYPE = ELF32_R_TYPE(reverseEndian32(realocationTable[j]->info));
            }
        }
    }
    return realocationTableF;
}
  //il faut extraire les données de chaque section de tab avec l'étape 3
  //les REL ont deux valeurs de 8 chiffres hexas décalage info
  //d'Info ont à:
  //les deux premières chiffres correspondent au type de réimplantation propre a l'architecture (processeur)
  //le reste des 6 chiffres hexas représentent l'index du symbole
  //voir: elf.pdf page 36 et ELF for the ARM architecture page 26

void afficherRealocationTab(RealocationEntryF** realocationTableF, int nbrEntry) {
  for (int j = 0; j < nbrEntry; j++) {
      printf("\toffset : %08X\n", realocationTableF[j]->offset);
      printf("\tsymbole index : %02X\n", realocationTableF[j]->SYM);
      printf("\ttype : %02X\n", realocationTableF[j]->TYPE);
  }
}
