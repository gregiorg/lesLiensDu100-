#include "legolas.h"

void stringTableAddString(SectionHeader* sectionHeader, char* string) {
    /*
		On vérifie que le section header actuel soit bien de type string table,
		et le cas échéant on rajoute la string passé en paramètre à la string
		table.
	*/

	if (sectionHeader->type == SHT_STRTAB) {
		int newIndex = sectionHeader->size;
		sectionHeader->size += strlen(string) + 1;
		sectionHeader->data.stringTable = realloc(sectionHeader->data.stringTable, sectionHeader->size);
        strcpy(&(sectionHeader->data.stringTable[newIndex]), string);
		//sectionHeader->data.stringTable[sectionHeader->size - 1] = 0;
    } 
	
	//Si le section header n'est pas une string table, on renvoie une erreur.
	
	else {
        printf("Error : stringTableAddString called on a non SHT_STRTAB sectionHeader\n");
    }
}

char* stringTableGetString(SectionHeader* sectionHeader, int index) {
	char* string = NULL;

	/*
		Si le section header actuel est de type string table, on renvoie
		l'index de la string passée en paramètre.
		Sinon, on print une erreur et on renvoie une string table.
	*/

    if (sectionHeader->type == SHT_STRTAB) {
		string = &(sectionHeader->data.stringTable[index]);
    } else {
        printf("Error : stringTableAddString called on a non SHT_STRTAB sectionHeader\n");
    }

	return string;
}

int stringTableGetIndex(SectionHeader* sectionHeader, char* string) {
    int i = 0;

	/*
		Si le section header actuel est bien de type string table, on cherche l'index
		de la string passée en paramètre.
		Sinon, on print une erreur et on renvoie -1.
	*/

    if (sectionHeader->type == SHT_STRTAB) {
		/*
			On incrémente i tant que la string passée en paramètre est différente de la string
			lue dans la string table.
		*/

		while (i < sectionHeader->size && strcmp(&(sectionHeader->data.stringTable[i]), string) != 0) {
			i += strlen(&(sectionHeader->data.stringTable[i])) + 1;
		}
	
		/*
			Si l'indice retourné est supérieur à la taille de la string table, ça veut dire
			qu'on n'a pas trouvé la string passée en paramètre dans la string table, donc on
			print une erreur et on renvoie -1.
		*/
		
		if (i >= sectionHeader->size) {
			printf("Error : \"%s\" is not present in stringTable %p\n", string, sectionHeader);
			i = -1;
		}
    } else {
    	printf("Error : stringTableGetIndex called on a non SHT_STRTAB sectionHeader\n");
		i = -1;
    }

    return i;
}

SectionHeader* stringTableCreate(char* name) {
    SectionHeader* stringTable = malloc(sizeof(SectionHeader));
    stringTable->name = name;
    stringTable->type = SHT_STRTAB;
    stringTable->size = 1;
	stringTable->data.stringTable = malloc(1);
	stringTable->data.stringTable[0] = 0;

    return stringTable;
}
