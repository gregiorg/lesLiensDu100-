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
        printf("Error : section header %p %s %i not found in header %p\n", sectionHeader, sectionHeader->name, sectionHeader->type, header);
        i = -1;
    }

    return i;
}

SectionHeader* getSectionHeaderFromName(Header* header, char* nomHeader) {
	unsigned int i = 0;

	/*
		 On boucle tant qu'on n'a pas trouvé le nom correspondant au nom transmis dans la section
		 header table ou qu'on n'a pas atteint la dernière section.
	*/

	while (i < header->shnum && (strcmp(header->sectionHeaderTable[i]->name, nomHeader) != 0)) {
		i++;
	}

	/*
		Si i est supérieur au nombre de section total, c'est qu'on n'a pas trouvé la section, donc
		on renvoie une erreur.
	*/

	if (i >= header->shnum) {
		printf("Error : The section header named %s was not found in the section header table. The link edition will stop.", nomHeader);
		exit(1);
	}

	return header->sectionHeaderTable[i];
}

SectionHeader** getSectionHeadersFromType(Header* header, uint32_t type, size_t* tailleListe) {
	SectionHeader** sectionHeaderList = NULL;
    *tailleListe = 0;

	/*
	 	On vérifie le type de chaque section présente dans la table des sections.
		S'il correspond au type transmis, on le rajoute à la liste des section header,
		et on incrémente la taille.
	*/

	for (int i=0; i < header->shnum; i++) {
		if (header->sectionHeaderTable[i]->type == type) {
			(*tailleListe)++;
			sectionHeaderList = realloc(sectionHeaderList, sizeof(SectionHeader*) * (*tailleListe));
			sectionHeaderList[(*tailleListe)-1] = header->sectionHeaderTable[i];
		}
	}

	return sectionHeaderList;
}
