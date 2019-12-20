#include "etape6.h"

int main(int argc, char const *argv[]) {

  FILE* file1 = fopen(argv[1], "r");
  FILE* file2 = fopen(argv[2], "r");
  FILE* fileOut = fopen(argv[3], "w");

  // get file Header
  ElfHeaderF* elfHeaderF1 = getElfHeader(file1);
  ElfHeaderF* elfHeaderF2 = getElfHeader(file2);

  //get sections
  ElfSecHeaderF** secHeaderF1 = getTabElfSecHeader(file1);
  ElfSecHeaderF** secHeaderF2 = getTabElfSecHeader(file2);

 //resultat fusion secHeaders
  ElfSecHeaderF** secHeaderO = malloc(sizeof(ElfSecHeaderF*)*(nbProgbitF1 + nbProgbitF2));
  void *rawData1, *rawData2;

  //comptage des progbits
  int nbProgbitF1 = countNbSecProgbits(secHeaderF1, elfHeaderF1->shnum);
  int nbProgbitF2 = countNbSecProgbits(secHeaderF2, elfHeaderF2->shnum);

  //Extractions des PROGBITS
  ElfSecHeaderF** secHeaderProgbitF1 = extractProgbit(secHeaderF1, elfHeaderF1->shnum, nbProgbitF1);
  ElfSecHeaderF** secHeaderProgbitF2 = extractProgbit(secHeaderF2, elfHeaderF2->shnum, nbProgbitF2);

  afficherTabSecHeader(secHeaderProgbitF1, nbProgbitF1);
  afficherTabSecHeader(secHeaderProgbitF2, nbProgbitF2);

  lireDonner();

  return 0;
}

int fusionSecHeader(ElfSecHeaderF** secHeaderProgbitF1, ElfSecHeaderF** secHeaderProgbitF1, int nbProgbitF1, int nbProgbitF2){

  for(int i; i < nbProgbitF1; i++){
    for(int j; j < nbProgbitF1; j++){
      if(strcmp(secHeaderProgbitF1[i]->nameStr,secHeaderProgbitF1[j]->nameStr)==0){
        if(addrAligncomp(secHeaderProgbitF1[i]->addrAlign, secHeaderProgbitF1[j]->addrAlign)){

          rawData1 = malloc(secHeaderProgbitF1[i]->size );
          rawData2 = malloc(secHeaderProgbitF2[j]->size);

          //lecture des donnée de progbit1
          fseek(file, reverseEndian32(secHeaderProgbitF1[i]->offset), SEEK_SET);
          fread(rawData1, secHeaderProgbitF1[i]->size, 1, file);

          //lecture des donnée de progbit2
          fseek(file, reverseEndian32(secHeaderProgbitF2[i]->offset), SEEK_SET);
          fread(rawData2, secHeaderProgbitF2[i]->size, 1, file);

          // deux vecteur indiquant le reste des PROGBITS
          //ecriture de du Header dans le fichier
          //Ecriture des secHeaderO dans le fichier.


        }
      }
    }
  }
  return 0;
}


int addrAligncomp(ElfSecHeaderF secHeaderProgbitF1 ,ElfSecHeaderF secHeaderProgbitF2){
  uint32_t  addrA1 = addrA1, addrA2 = secHeaderProgbitF2->addrAlign;

  if(addrA1<2 && addrA2<2 )
    return addrA1; // choix arbitraire
  else if(addrA1 < 2 && addrA2 >= 2)
    return addrA2;
  else if(addrA1 >= 2 && addrA2 < 2)
    return addrA1;
  else if(addrA1 == addrA2)
    return addrA1;
  else if(addrA1 >= 2 && addrA2 >= 2)
    printf("Tailles des mots des deux PROGBITS differentes\n");

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
