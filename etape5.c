

Elf_SecHeaderF* etape5(char* fname)

Elf_SecHeaderF tab[] = etape2(fname);

  for(int i=0; i<n ; i++){
    if(tab[i].typeInt==9 ){  // 9 correspond a type REL
      Elf_SecHeaderF Ntab[j]=tab[i];
      j++;
    }
  }

  for(int i=0 ; i<n ; i++){
    printf("Offset : %d", Ntab[i].offset)
    uint32_t line = etape3(fnam, Ntab[i].indexTable);


    Elf32_Sym etap4(); // st_shndx

    printf("%d\n", line);
  }

}
