#include "legolas.h"

void setHeader(Elf32_Ehdr* fileHeader, Header* header) {
	//On ajoute toutes les informations au header courant

	header->indentClass = fileHeader->e_ident[EI_CLASS];
    header->indentData  = fileHeader->e_ident[EI_DATA];
	programsEndian = fileHeader->e_ident[EI_DATA];
    header->indentVersion = fileHeader->e_ident[EI_VERSION];
    header->indentOSABI = fileHeader->e_ident[EI_OSABI];
    header->indentABIVersion = fileHeader->e_ident[EI_ABIVERSION];
    header->type = reverseEndian16(fileHeader->e_type);
    header->machine = reverseEndian16(fileHeader->e_machine);
    header->version = reverseEndian32(fileHeader->e_version);
    header->entry = reverseEndian32(fileHeader->e_entry);
    header->flags = reverseEndian32(fileHeader->e_flags);
    header->shnum = reverseEndian16(fileHeader->e_shnum);
}

void headerAddSection(Header* header, SectionHeader* sectionHeader) {
	//On ajoute un section header au header actuel.
	
	header->shnum++;
    header->sectionHeaderTable = realloc(header->sectionHeaderTable, header->shnum * sizeof(SectionHeader*));
    header->sectionHeaderTable[header->shnum-1] = sectionHeader;
}

void headerRemoveSectionHeader(Header* header, SectionHeader* sectionHeader) {
	/*
		On supprime le section header actuel de la liste de section header
		en remplaçant le section header à supprimer par le dernier et en
		réduisant le nombre de section header de 1.
	*/
	
	int i = headerGetIndexOfSectionHeader(header, sectionHeader);
	header->sectionHeaderTable[i] = header->sectionHeaderTable[header->shnum-1];
	header->shnum--;
	//free(sectionHeader);
}

int headerGetIndexOfSectionHeader(Header* header, SectionHeader* sectionHeader) {
    int i = 0;

	//On cherche l'index du section header actuel dans la liste des section header.

    while (i < header->shnum && header->sectionHeaderTable[i] != sectionHeader) {
        i++;
    }

	/*
		Si l'index renvoyé est supérieur au nombre de sections renseigné dans le header,
		c'est qu'on n'a pas trouvé le section header passé en paramètre, donc on renvoie
		une erreur.
	*/

    if (i >= header->shnum) {
        printf("Error : section header %p not found in header %p\n", sectionHeader, header);
        i = -1;
    }

    return i;
}
