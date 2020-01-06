#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "legolas.h"

/*
void setHeader(ElfHeader elfHeader, Header* header) {
	header->indentClass = elfHeader.indentClass;
    header->indentData = elfHeader.indentData;
    header->indentVersion = elfHeader.indentVersion;
    header->type = reverseEndian16(elfHeader.type);
    header->machine = reverseEndian16(elfHeader.machine);
    header->version = reverseEndian32(elfHeader.version);
    header->entry = reverseEndian32(elfHeader.entry);
    header->flags = reverseEndian32(elfHeader.flags);
	header->nbSections = reverseEndian16(elfHeader.shnum);
}

void setSectionHeader(Header* header, ElfSecHeader* elfSecHeaders, int i, FILE* file) {
	SectionHeader* currentSectionHeader = malloc(sizeof(SectionHeader));
	ElfSecHeader currentElfSecHeader = elfSecHeaders[i];

	currentSectionHeader->indexTable = i;
	currentSectionHeader->type = reverseEndian32(currentElfSecHeader.type);
	currentSectionHeader->flags = reverseEndian32(currentElfSecHeader.flags);
	currentSectionHeader->addr = reverseEndian32(currentElfSecHeader.addr);
	currentSectionHeader->link = reverseEndian32(currentElfSecHeader.link);
	currentSectionHeader->info = reverseEndian32(currentElfSecHeader.info);
	currentSectionHeader->addrAlign = reverseEndian32(currentElfSecHeader.addrAlign);
	currentSectionHeader->entSize = reverseEndian32(currentElfSecHeader.entSize);
	currentSectionHeader->size = reverseEndian32(currentElfSecHeader.size);

	currentSectionHeader->rawData = malloc(currentSectionHeader->size);
	fseek(file, reverseEndian32(currentElfSecHeader.offset), SEEK_SET);
	fread(currentSectionHeader->rawData, currentSectionHeader->size, 1, file);

	typeRawDataIfNeeded(currentSectionHeader, header);

	header->sections[i] = currentSectionHeader;
}

void typeRawDataIfNeeded(SectionHeader* currentSectionHeader, Header* header) {

	switch(currentSectionHeader->type) {
		case SHT_STRTAB: {
			currentSectionHeader->data.stringTable = (char*) currentSectionHeader->rawData;
			break;
		}

		case SHT_REL: {
			uint32_t nbRelocationTableEntry = (currentSectionHeader->size / currentSectionHeader->entSize);

			currentSectionHeader->data.relocationTable = malloc (sizeof(void*) * nbRelocationTableEntry);

			for (int i=0; i < nbRelocationTableEntry; i++) {
				RelocationTableEntry* currentRelocationTableEntry = currentSectionHeader->data.relocationTable[i];

				currentRelocationTableEntry = malloc (sizeof(RelocationTableEntry));

				RealocationEntry* relocationEntry;

				relocationEntry = (RealocationEntry*) &(((char*) currentSectionHeader->rawData)[currentSectionHeader->entSize * i]);

				currentRelocationTableEntry->type = ELF32_R_TYPE(relocationEntry->info);
				currentRelocationTableEntry->sym = getSymboleTableEntryAddress(header, ELF32_R_SYM(relocationEntry->info));
			}

			break; }
		case SHT_SYMTAB: {
			uint32_t nbSymbolTableEntry = (currentSectionHeader->size / currentSectionHeader->entSize);

			currentSectionHeader->data.symboleTable = malloc (sizeof(void*) * nbSymbolTableEntry);

			for (int i=0; i < nbSymbolTableEntry; i++) {
				SymboleTableEntry* currentSymbolTableEntry = currentSectionHeader->data.symboleTable[i];

				currentSymbolTableEntry = malloc (sizeof(SymboleTableEntry));

				Elf32Sym* elf32Sym;

				elf32Sym = (Elf32Sym*) &(((char*) currentSectionHeader->rawData)[currentSectionHeader->entSize * i]);

				currentSymbolTableEntry->value = elf32Sym->stValue;
				currentSymbolTableEntry->size = elf32Sym->stSize;
				currentSymbolTableEntry->other = elf32Sym->stOther;
				currentSymbolTableEntry->bind = ELF32_ST_BIND(elf32Sym->stInfo);
				currentSymbolTableEntry->type = ELF32_ST_TYPE(elf32Sym->stInfo);
				currentSymbolTableEntry->sectionHeader = getSectionHeaderAddress(header, elf32Sym->stShndx);
				currentSymbolTableEntry->name = getSymbolTableEntryName(header, elf32Sym->stName);
			}

			break;
		}
	}
}

char* getSymbolTableEntryName(Header* header, uint32_t indexName) {
	unsigned int indexStringTable = 0;

	while(indexStringTable < header->nbSections && strcmp(header->sections[indexStringTable]->name, ".strtab") != 0) {
    	indexStringTable++;
	}

	return ((header->sections[indexStringTable]->data.stringTable) + indexName);
}

SymboleTableEntry* getSymboleTableEntryAddress(Header* header, uint32_t info) {
	unsigned int i = 0;

	while (header->sections[i]->type != SHT_SYMTAB) {
		i++;
	}

	return header->sections[i]->data.symboleTable[i];
}

SectionHeader* getSectionHeaderAddress(Header* header, uint16_t shndx) {
	return header->sections[shndx];
}
*/

Header* legolasReadFromFile(FILE* file) {
	//lecture du header
    Elf32_Ehdr fileHeader;
    fseek(file, 0, SEEK_SET);
    fread(&fileHeader, sizeof(fileHeader), 1, file);

    Header* header = malloc(sizeof(Header));
    header->indentClass = fileHeader.e_ident[EI_CLASS];
    header->indentData  = fileHeader.e_ident[EI_DATA];
    header->indentVersion = fileHeader.e_ident[EI_VERSION];
    header->indentOSABI = fileHeader.e_ident[EI_OSABI];
    header->indentABIVersion = fileHeader.e_ident[EI_ABIVERSION];
    header->type = reverseEndian16(fileHeader.e_type);
    header->machine = reverseEndian16(fileHeader.e_machine);
    header->version = reverseEndian32(fileHeader.e_version);
    header->entry = reverseEndian32(fileHeader.e_entry);
    header->flags = reverseEndian32(fileHeader.e_flags);
    header->shnum = reverseEndian16(fileHeader.e_shnum);
    header->sectionHeaderTable = malloc(sizeof(SectionHeader*) * header->shnum);

	//lecture de la table de sections
	int fileSectionHeaderTableSize = sizeof(Elf32_Shdr) * header->shnum;
    Elf32_Shdr* fileSectionHeaderTable = malloc(fileSectionHeaderTableSize);
    fseek(file, reverseEndian32(fileHeader.e_shoff), SEEK_SET);
    fread(fileSectionHeaderTable, fileSectionHeaderTableSize, 1, file);

    for (int i = 0; i < header->shnum; i++) {
	    header->sectionHeaderTable[i] = malloc(sizeof(SectionHeader));

	    SectionHeader* sectionHeader = header->sectionHeaderTable[i];
	    Elf32_Shdr* fileSectionHeader = &(fileSectionHeaderTable[i]);

	    sectionHeader->type = reverseEndian32(fileSectionHeader->sh_type);
	    sectionHeader->flags = reverseEndian32(fileSectionHeader->sh_flags);
	    sectionHeader->addr = reverseEndian32(fileSectionHeader->sh_addr);
	    sectionHeader->link = reverseEndian32(fileSectionHeader->sh_link);
	    sectionHeader->info = reverseEndian32(fileSectionHeader->sh_info);
	    sectionHeader->addrAlign = reverseEndian32(fileSectionHeader->sh_addralign);
	    sectionHeader->size = reverseEndian32(fileSectionHeader->sh_size);
		sectionHeader->data.raw = malloc(sectionHeader->size);

		fseek(file, reverseEndian32(fileSectionHeader->sh_offset), SEEK_SET);
	    fread(sectionHeader->data.raw, sectionHeader->size, 1, file);
    }

	//remplissage noms de header de section
	int shstrndx = reverseEndian16(fileHeader.e_shstrndx);
	SectionHeader* stringTableSectionHeader = header->sectionHeaderTable[shstrndx];

    for (int i = 0; i < header->shnum; i++) {
		SectionHeader* sectionHeader = header->sectionHeaderTable[i];
	    Elf32_Shdr* fileSectionHeader = &(fileSectionHeaderTable[i]);

		char* name = stringTableGetString(stringTableSectionHeader, reverseEndian32(fileSectionHeader->sh_name));
		sectionHeader->name = malloc(strlen(name));
        strcpy(sectionHeader->name, name);
    }

	//suppression de la trable des chaines
	headerRemoveSectionHeader(header, stringTableSectionHeader);

	return header;
}

void headerAddSection(Header* header, SectionHeader* sectionHeader) {
    header->shnum++;
    header->sectionHeaderTable = realloc(header->sectionHeaderTable, header->shnum * sizeof(SectionHeader*));
    header->sectionHeaderTable[header->shnum-1] = sectionHeader;
}

int headerGetIndexOfSectionHeader(Header* header, SectionHeader* sectionHeader) {
    int i = 0;

    while (i < header->shnum && header->sectionHeaderTable[i] != sectionHeader) {
        i++;
    }

    if (i >= header->shnum) {
        printf("Error : section header %p not found in header %p\n", sectionHeader, header);
        i = -1;
    }

    return i;
}

void headerRemoveSectionHeader(Header* header, SectionHeader* sectionHeader) {
		int i = headerGetIndexOfSectionHeader(header, sectionHeader);
		header->sectionHeaderTable[i] = header->sectionHeaderTable[header->shnum-1];
		header->shnum--;
		free(sectionHeader);
}

void stringTableAddString(SectionHeader* sectionHeader, char* string) {
    if (sectionHeader->type == SHT_STRTAB) {
		int newIndex = sectionHeader->size;
		sectionHeader->size += strlen(string) + 1;
		sectionHeader->data.raw = realloc(sectionHeader->data.raw, sectionHeader->size);
        strcpy(&(sectionHeader->data.stringTable[newIndex]), string);
		//sectionHeader->data.stringTable[sectionHeader->size - 1] = 0;
    } else {
        printf("Error : stringTableAddString called on a non SHT_STRTAB sectionHeader\n");
    }
}

char* stringTableGetString(SectionHeader* sectionHeader, int index) {
	char* string = NULL;

    if (sectionHeader->type == SHT_STRTAB) {
		string = &(sectionHeader->data.stringTable[index]);
    } else {
        printf("Error : stringTableAddString called on a non SHT_STRTAB sectionHeader\n");
    }

	return string;
}

int stringTableGetIndex(SectionHeader* sectionHeader, char* string) {
    int i = 0;

    if (sectionHeader->type == SHT_STRTAB) {
			while (i < sectionHeader->size && strcmp(&(sectionHeader->data.stringTable[i]), string) != 0) {
					i += strlen(&(sectionHeader->data.stringTable[i])) + 1;
			}

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

char* sectionHeaderGetData(SectionHeader* sectionHeader) {
		return (char*) sectionHeader->data.raw;
}

uint32_t sectionHeaderGetEntSize(SectionHeader* sectionHeader) {
		return 1;
}

void legolasWriteToFile(Header* header, FILE* file) {
    //creation de la table des chaines
    SectionHeader* stringTableSectionHeader = malloc(sizeof(SectionHeader));
    stringTableSectionHeader->name = ".shstrtab";
    stringTableSectionHeader->type = SHT_STRTAB;
    stringTableSectionHeader->size = 0;
	//stringTableSectionHeader->data.raw = malloc(1);
	//stringTableSectionHeader->data.stringTable[0] = 0;
    headerAddSection(header, stringTableSectionHeader);

    //remplissage de la table des chaines
    for (int i = 0; i < header->shnum; i++) {
	    SectionHeader* sectionHeader = header->sectionHeaderTable[i];
	    stringTableAddString(stringTableSectionHeader, sectionHeader->name);
    }

	//creation du header
    Elf32_Ehdr fileHeader;
    fileHeader.e_ident[EI_MAG0] = ELFMAG0;
    fileHeader.e_ident[EI_MAG1] = ELFMAG1;
    fileHeader.e_ident[EI_MAG2] = ELFMAG2;
    fileHeader.e_ident[EI_MAG3] = ELFMAG3;
    fileHeader.e_ident[EI_CLASS] = header->indentClass;
    fileHeader.e_ident[EI_DATA] = header->indentData ;
    fileHeader.e_ident[EI_VERSION] = header->indentVersion;
    fileHeader.e_ident[EI_OSABI] = header->indentOSABI;
    fileHeader.e_ident[EI_ABIVERSION] = header->indentABIVersion;
    fileHeader.e_type = reverseEndian16(header->type);
    fileHeader.e_machine = reverseEndian16(header->machine);
    fileHeader.e_version = reverseEndian32(header->version);
    fileHeader.e_entry = reverseEndian32(header->entry);
    fileHeader.e_flags = reverseEndian32(header->flags);
    fileHeader.e_ehsize = reverseEndian16(sizeof(fileHeader));
    fileHeader.e_shnum = reverseEndian16(header->shnum);
	fileHeader.e_shentsize = reverseEndian16(sizeof(Elf32_Shdr));
    fileHeader.e_shstrndx = reverseEndian16(headerGetIndexOfSectionHeader(header, stringTableSectionHeader));
    fileHeader.e_phoff = 0;
    fileHeader.e_phentsize = 0;
    fileHeader.e_phnum = 0;

    //on laisse de la place dans le fichier pour le header
    fseek(file, sizeof(fileHeader), SEEK_SET);

    //creation de la table des sections
	int fileSectionHeaderTableSize = sizeof(Elf32_Shdr) * header->shnum;
    Elf32_Shdr* fileSectionHeaderTable = malloc(fileSectionHeaderTableSize);

    //ecriture des sections dans le fichier et memorisation des offsets
    for (int i = 0; i < header->shnum; i++) {
	    SectionHeader* sectionHeader = header->sectionHeaderTable[i];
	    Elf32_Shdr* fileSectionHeader = &(fileSectionHeaderTable[i]);

	    fileSectionHeader->sh_name = reverseEndian32(stringTableGetIndex(stringTableSectionHeader, sectionHeader->name));
	    fileSectionHeader->sh_type = reverseEndian32(sectionHeader->type);
	    fileSectionHeader->sh_flags = reverseEndian32(sectionHeader->flags);
	    fileSectionHeader->sh_addr = reverseEndian32(sectionHeader->addr);
	    fileSectionHeader->sh_link = reverseEndian32(sectionHeader->link);
	    fileSectionHeader->sh_info = reverseEndian32(sectionHeader->info);
	    fileSectionHeader->sh_addralign = reverseEndian32(sectionHeader->addrAlign);
	    fileSectionHeader->sh_size = reverseEndian32(sectionHeader->size);
	    fileSectionHeader->sh_entsize = reverseEndian32(sectionHeaderGetEntSize(sectionHeader));
	    fileSectionHeader->sh_offset = reverseEndian32(ftell(file));
	    fwrite(sectionHeaderGetData(sectionHeader), sectionHeader->size, 1, file);
    }

    //ecriture de la table des sections
    fileHeader.e_shoff = reverseEndian32(ftell(file));
    fwrite(fileSectionHeaderTable, fileSectionHeaderTableSize, 1, file);

    //eciture du header
    fseek(file, 0, SEEK_SET);
    fwrite(&fileHeader, sizeof(fileHeader), 1, file);
}

/*
int main(int argc, char** argv) {
	FILE* file1 = fopen(argv[1], "r");
	FILE* file2 = fopen(argv[2], "w");

	if (file1 == NULL || file2 == NULL) {
		return 1;
	}

	Header* header = legolasReadFromFile(file1);
	legolasWriteToFile(header, file2);

	fclose(file1);
	fclose(file2);

	return 0;
}
*/
