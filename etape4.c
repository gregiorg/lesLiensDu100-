//----------------AFFICHAGE DE LA TABLE DES SYMBOLES----------------------------
#include "etape4.h"

Elf32Sym** getTabSym(FILE* f) {
  long int filePos = ftell(f);
  ElfHeaderF* elfHeader = getElfHeader(f);  // get the header of the elf file
  ElfSecHeaderF** elfSecHeader = getTabElfSecHeader(f); // get all section headers

  int a = 0;
  while(a < elfHeader->shnum && strcmp(elfSecHeader[a]->nameStr, ".strtab")!=0) {
    a++;
  }  // finding the string tables offset for the following line

  // uint32_t stringTableAddress = getAddressStringTable(elfHeader->shoff, elfHeader->shentsize, elfSecHeader[a]->offset, f);  // string table addr for later use

  // Find the symboles table section headers index; stored in i
  int i = 0;
  while(i < elfHeader->shnum && strcmp(elfSecHeader[i]->nameStr, ".symtab")!=0) {
    i++;
  }

  uint32_t* data = readDataNomSec(f, ".symtab");  // get the symboles table AS RAW DATA and store it in data

  int nbElm = (elfSecHeader[i]->size) / (elfSecHeader[i]->entSize);  // number of elements in the symboles table

  // allocating memory to store the symboles table as structs
  Elf32Sym** tabSym = malloc(nbElm * sizeof(Elf32Sym*));

  for(int k = 0; k < nbElm; k++) {
    tabSym[k] = malloc(sizeof(Elf32Sym));
  }

  // extracting raw data and storing it as a table of pointers of structs.
  for (int j = 0; j < nbElm; j++) {
    tabSym[j]->stName = reverseEndian32(data[j*4]);
    tabSym[j]->stValue = reverseEndian32(data[j*4+1]);
    tabSym[j]->stSize = reverseEndian32(data[j*4+2]);

    uint32_t sauv = reverseEndian32(data[j*4+3]);
    tabSym[j]->stInfo = sauv >> 24;
    tabSym[j]->stOther = (sauv >> 16) & 0x00FF;
    tabSym[j]->stShndx = sauv & 0xFFFF;
    // ^see in util.h the way the struct is constructed^
  }
  // afficherTabSym(tabSym, nbElm, stringTableAddress, f);
  fseek(f, filePos, SEEK_SET);
  return tabSym;
}

//Affichage
void afficherTabSym(Elf32Sym** tabSym, int size, uint32_t stringTableAddress, FILE* f) {
  for (int i = 0; i < size; i++) {
    printf("Symbole n°%d :\n", i);
    printf("	Indice du nom du symbole : %08X\n", tabSym[i]->stName);
    printf("	Nom du symbole : %s\n", showName(tabSym[i]->stName, stringTableAddress, f));
    printf("	Valeur du symbole : %08X\n", tabSym[i]->stValue);
    printf("	Taille du symbole : %08X\n", tabSym[i]->stSize);
    printf("	NDX du symbole : %d\n", tabSym[i]->stShndx);
    printf("\n");
  }
}
