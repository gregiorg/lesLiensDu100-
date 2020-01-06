#include "legolas.h"
#include <elf.h>

int main(int argc, char const *argv[]) {

  FILE* file1 = fopen(argv[1], "r");
  FILE* file2 = fopen(argv[2], "r");
  FILE* file3 = fopen(argv[3], "w");

  Header* h1 = legolasReadFromFile(file1);
  Header* h2 = legolasReadFromFile(file2);

  // integration des sections du header 2 dans le header 1
  for (int i = 0; i < h2->shnum; i++) {
      SectionHeader* sh2 = h2->sectionHeaderTable[i];

      int progbitFusion = 0;

      // fusion des progbits de meme nom
      for (int j = 0; j < h1->shnum; j++) {
          SectionHeader* sh1 = h1->sectionHeaderTable[j];

          if (sh1->type == SHT_PROGBITS && sh2->type == SHT_PROGBITS && strcmp(sh1->name, sh2->name)==0) {
              progbitFusion = 1;

              sh1->rawData = realloc(sh1->rawData, sh1->size + sh2->size);
              memcpy(((char*) sh1->rawData) + sh1->size, sh2->rawData, sh2->size);
              sh1->size += sh2->size;

              break;
          }
      }

      // sinon ajout de la section
      if (progbitFusion == 0) {
          headerAddSection(h1, sh2);
      }
  }

  legolasWriteToFile(h1, file3);

  return 0;
}

/*
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
*/
