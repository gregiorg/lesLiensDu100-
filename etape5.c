#define ELF32_R_SYM(info)    ((info)>>8)
#define ELF32_R_TYPE(info)   ((unsigned char)(info))
//on prend tab de l'etape 2
//on extrait les REL du tab
ElfSecHeaderF* etape5(char* fname)

ElfSecHeaderF tab[] = etape2(fname);

  for(int i=0; i<n ; i++){
    if(tab[i].typeInt==9 ){  // 9 correspond a type REL
      ElfSecHeaderF Ntab[j]=tab[i];
      j++;
    }
  }


  //il faut extraire les données de chaque section de tab avec l'étape 3
  //les REL ont deux valeurs de 8 chiffres hexas décalage info
  //d'Info ont à:
  //les deux premières chiffres correspondent au type de réimplantation propre a l'architecture (processeur)
  //le reste des 6 chiffres hexas représentent l'index du symbole
  //voir: elf.pdf page 36 et ELF for the ARM architecture page 26


  for(int i=0 ; i<n ; i++){
    printf("Offset : %d", Ntab[i].offset)
    uint32_t line = etape3(fnam, Ntab[i].indexTable);

    Elf32Sym etap4(); // st_shndx

    printf("%d\n", line);
  }

}
