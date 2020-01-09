Custom ELF Loader intended for pedagogic use

Contributors :                          Role
        AIT GUENI SSAID Abderrahmane    ()
        BOURNAT-QUERAT Jean-Edouard     (CHRO)
        CIPRIANI Ugo                    ()
        FODOR Gergely                   ()
        NIVON Quentin                   ()
        RICHERMOZ Antoine               ()

Use :
    Clone the project with : `git clone https://github.com/gregiorg/lesLiensDu100-.git`
    Then set your current directory to the project directory and `make`.
    You can now use the full project :
        Combine two object files (.o) into one : `./fusion file1.o file2.o newFile.o`
        Display a .o header : `./readElfHeader file.o`
        Display a .o section table : `./readElfSecTab file.o`
        Display a .o section raw data : `./readElfSecRawD file.o 1` (where 1 is a section index)
                                        or `./readElfSecRawD file.o .strtab` (where .strtab is a section name)
        Display a .o symbol table : `./readElfSymTab file.o`
        Display a .o relocation table : `./readElfRelTab file.o`

Le projet :
    Pour ce projet,  