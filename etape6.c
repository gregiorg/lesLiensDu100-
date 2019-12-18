#include "etape6.h"

int main(int argc, char const *argv[]) {

  FILE* f1 = fopen(argv[1], "r");
  FILE* f2 = fopen(argv[2], "r");
  FILE* fOut = fopen(argv[3], "w");

  ElfHeaderF* elfHeaderF1 = getElfHeader(f1);
  ElfHeaderF* elfHeaderF2 = getElfHeader(f2);

  ElfSecHeaderF** secHeaderF1 = getTabElfSecHeader(f1);
  ElfSecHeaderF** secHeaderF2 = getTabElfSecHeader(f2);

  int nbProgbitF1 = countNbSecProgbits(secHeaderF1, elfHeaderF1->shnum);
  int nbProgbitF2 = countNbSecProgbits(secHeaderF2, elfHeaderF2->shnum);

  ElfSecHeaderF** secHeaderProgbitF1 = extractProgbit(secHeaderF1, elfHeaderF1->shnum, nbProgbitF1);
  ElfSecHeaderF** secHeaderProgbitF2 = extractProgbit(secHeaderF2, elfHeaderF2->shnum, nbProgbitF2);

  afficherTabSecHeader(secHeaderProgbitF1, nbProgbitF1);
  afficherTabSecHeader(secHeaderProgbitF2, nbProgbitF2);

  return 0;
}

int countNbSecProgbits(ElfSecHeaderF** allSecHeaders, int nbSecHeaders) {
  int nbSecProgbit = 0;
  for (int i = 0; i < nbSecHeaders; i++) {
    if (allSecHeaders[i]->typeInt == 1) {  // PROGBITS == 1
      nbSecProgbit++;
    }
  }
  return nbSecProgbit;
}

ElfSecHeaderF** allocSecHeader(int nbElem) {
  ElfSecHeaderF** secHeader = malloc(nbElem * sizeof(ElfSecHeaderF*));
  for (int i = 0; i < nbElem; i++) {
    secHeader[i] = malloc(sizeof(ElfSecHeaderF));
  }
  return secHeader;
}

ElfSecHeaderF** extractProgbit(ElfSecHeaderF** allSecHeaders, int nbAllSecHeaders, int nbSecProgbit) {
  ElfSecHeaderF** secHeaderProgbit = allocSecHeader(nbSecProgbit);

  int j = 0;
  for (int i = 0; i < nbAllSecHeaders; i++) {
    if (allSecHeaders[i]->typeInt == 1) {  // PROGBITS == 1
      secHeaderProgbit[j] = allSecHeaders[i];
      j++;
    }
  }
  return secHeaderProgbit;
}
