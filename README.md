Custom ELF Loader intended for pedagogic use

Contributors :
        AIT GUENI SSAID Abderrahmane
        BOURNAT-QUERAT Jean-Edouard (CHRO)
        CIPRIANI Ugo
        FODOR Gergely
        NIVON Quentin
        RICHERMOZ Antoine

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
    Pour ce projet, nous avons tout d'abord fait une réunion pour prendre connaissance du sujet et échanger tous ensemble sur la démarche à suivre.
    Suite à cela, nous avons commencé le développement. De manière générale, nous avons travaillé avec une méthode se rapprochant de la méthode agile de l'Extreme Programming.
    La plupart du temps nous travaillions à deux sur une machine pour éviter un maximum les erreurs d'innattention qui sont régulières lorsque l'on travaille seul.
    De plus, cela permet de mettre deux cerveaux à contribution sur une tâche, rendant ainsi sa réalisation plus efficiente.
    Nous avions commencé en développant chaque étape indépendamment les unes des autres, puis nous sommes repassées dessus pour les rendre importables dans les étapes suivantes.
    Par la même occasion nous avons décidé d'une convention de nomenclatures présente dans le fichier "CONTRIBUTING.md".
    Enfin, en arrivant à l'étape 6, nous nous sommes rendus compte que traité le sujet de façon "objet" serait bien plus simple, nous avons donc décidé de créer une structure se rapprochant de la programmation objet et nous permettant de lire le fichier ELF seulement une fois.
    Cette structure nommée "Legolas" nous a grandement facilité la tâche pour la suite du projet car nous avions préparé la lecture du fichier en amont et il nous suffisait donc de récupéré les données directement dans la structure pour les réécrire.
    Nous sommes parvenus au bout du projet, l'étape 9 a été réalisé dans la même fonction que l'étape 8.