#include "util.h"
#include "etape2.h"

#define ELF32_R_SYM(info)    ((info)>>8)
#define ELF32_R_TYPE(info)   ((unsigned char)(info))

#define SH_REL 9

//on prend tab de l'etape 2
//on extrait les REL du tab

typedef struct {
    uint32_t offset;
    uint32_t info;
} RealocationEntry;

typedef struct {
    uint32_t offset;
    char SYM;
    char TYPE;
} RealocationEntryF;

void etape5(FILE* file) {
    ElfHeaderF* header = getElfHeader(file);
    ElfSecHeaderF** sectionTable = getTabElfSecHeader(file);

    for (int i = 0; i < header->shnum; i++) {
        ElfSecHeaderF* sectionHeader = sectionTable[i];

        if (sectionHeader->typeInt == SH_REL) {
            printf("RealocationTable : %08X\n", sectionHeader->offset);

            int nbrEntry = sectionHeader->size / sectionHeader->entSize;

            RealocationEntry* realocationTable = malloc(sizeof(RealocationEntry) * nbrEntry);
            fseek(file, sectionHeader->offset, SEEK_SET);
            fread(realocationTable, sizeof(RealocationEntry), nbrEntry, file);

            RealocationEntryF* realocationTableF = malloc(sizeof(RealocationEntryF) * nbrEntry);

            for (int j = 0; j < nbrEntry; j++) {
                realocationTableF[j].offset = reverseEndian32(realocationTable[j].offset);
                realocationTableF[j].SYM = ELF32_R_SYM(reverseEndian32(realocationTable[j].info));
                realocationTableF[j].TYPE = ELF32_R_TYPE(reverseEndian32(realocationTable[j].info));
            }

            for (int j = 0; j < nbrEntry; j++) {
                printf("\toffset : %08X\n", realocationTableF[j].offset);
                printf("\tsymbole index : %02X\n", realocationTableF[j].SYM);
                printf("\ttype : %02X\n", realocationTableF[j].TYPE);
            }
        }
    }
}

int main(int argc, char** argv) {
    FILE* file = fopen(argv[1], "r");
    etape5(file);
    fclose(file);

    return 0;
}

  //il faut extraire les données de chaque section de tab avec l'étape 3
  //les REL ont deux valeurs de 8 chiffres hexas décalage info
  //d'Info ont à:
  //les deux premières chiffres correspondent au type de réimplantation propre a l'architecture (processeur)
  //le reste des 6 chiffres hexas représentent l'index du symbole
  //voir: elf.pdf page 36 et ELF for the ARM architecture page 26
