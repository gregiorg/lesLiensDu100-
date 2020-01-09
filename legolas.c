#include "legolas.h"

Header* legolasReadFromFile(FILE* file) {
	//lecture du header
    Elf32_Ehdr fileHeader;
    fseek(file, 0, SEEK_SET);
    fread(&fileHeader, sizeof(fileHeader), 1, file);

    Header* header = malloc(sizeof(Header));

    setHeader(&fileHeader, header);

    header->sectionHeaderTable = malloc(sizeof(SectionHeader*) * header->shnum);

	//lecture de la table de sections
	int fileSectionHeaderTableSize = sizeof(Elf32_Shdr) * header->shnum;
    Elf32_Shdr* fileSectionHeaderTable = malloc(fileSectionHeaderTableSize);
    fseek(file, reverseEndian32(fileHeader.e_shoff), SEEK_SET);
    fread(fileSectionHeaderTable, fileSectionHeaderTableSize, 1, file);

    for (int i = 0; i < header->shnum; i++) {
        setSectionHeader(header, fileSectionHeaderTable, i, file);
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

    for (int i=0; i < header->shnum; i++) {
        typeLastRawDataPartIfNeeded(header->sectionHeaderTable[i], header);
    }

	//suppression de la trable des chaines
	headerRemoveSectionHeader(header, stringTableSectionHeader);

	return header;
}

void legolasWriteToFile(Header* header, FILE* file) {
	for (int i=0; i < header->shnum; i++) {
		if (header->sectionHeaderTable[i]->type == SHT_STRTAB) {
			headerRemoveSectionHeader(header, header->sectionHeaderTable[i]);
			i--;
		}
	}

	//creation de la table des chaines des symbol table entry
    SectionHeader* stringTableSymbolTableEntry = stringTableCreate(".strtab");
    headerAddSection(header, stringTableSymbolTableEntry);

    //remplissage de la table des chaines des symbol table entry
    for (int i = 0; i < header->shnum; i++) {
	    SectionHeader* sectionHeader = header->sectionHeaderTable[i];

		if (sectionHeader->type == SHT_SYMTAB) {
	    	for (int j=0; j < sectionHeader->nbEntry; j++) {
				stringTableAddString(stringTableSymbolTableEntry, sectionHeader->data.symboleTable[j]->name);
			}
		}
    }

    //creation de la table des chaines des section header
    SectionHeader* stringTableSectionHeader = stringTableCreate(".shstrtab");
    headerAddSection(header, stringTableSectionHeader);

    //remplissage de la table des chaines des section header
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

    SectionHeader* symbolTableSectionHeader = NULL;
    for (int i = 0; i < header->shnum; i++) {
        if (header->sectionHeaderTable[i]->type == SHT_SYMTAB) {
            symbolTableSectionHeader = header->sectionHeaderTable[i];
            break;
        }
    }

    //ecriture des sections dans le fichier et memorisation des offsets
    for (int i = 0; i < header->shnum; i++) {
	    SectionHeader* sectionHeader = header->sectionHeaderTable[i];
	    Elf32_Shdr* fileSectionHeader = &(fileSectionHeaderTable[i]);

	    fileSectionHeader->sh_name = reverseEndian32(stringTableGetIndex(stringTableSectionHeader, sectionHeader->name));
	    fileSectionHeader->sh_type = reverseEndian32(sectionHeader->type);
	    fileSectionHeader->sh_flags = reverseEndian32(sectionHeader->flags);
	    fileSectionHeader->sh_addr = reverseEndian32(sectionHeader->addr);
	    fileSectionHeader->sh_info = reverseEndian32(sectionHeader->info);
	    fileSectionHeader->sh_addralign = reverseEndian32(sectionHeader->addrAlign);
	    fileSectionHeader->sh_size = reverseEndian32(sectionHeader->size);
	    fileSectionHeader->sh_entsize = reverseEndian32(sectionHeader->entSize);
	    fileSectionHeader->sh_offset = reverseEndian32(ftell(file));
        fileSectionHeader->sh_link = 0;

		if (sectionHeader->type == SHT_SYMTAB) {
			fileSectionHeader->sh_size = reverseEndian32(sectionHeader->entSize * sectionHeader->nbEntry);
			fileSectionHeader->sh_link = reverseEndian32(headerGetIndexOfSectionHeader(header, stringTableSymbolTableEntry));
			fwrite(sectionHeaderGetSymbolData(header, sectionHeader, stringTableSymbolTableEntry), sectionHeader->size, 1, file);
		} else if (sectionHeader->type == SHT_REL) {
			fwrite(sectionHeaderGetUnexplicitRelocationData(sectionHeader, symbolTableSectionHeader), sectionHeader->size, 1, file);
            fileSectionHeader->sh_size = reverseEndian32(sectionHeader->size);
            fileSectionHeader->sh_link = reverseEndian32(headerGetIndexOfSectionHeader(header, symbolTableSectionHeader));
            fileSectionHeader->sh_info = reverseEndian32(headerGetIndexOfSectionHeader(header, sectionHeader->data.relocationTable[0]->sym->sectionHeader));
		} else if (sectionHeader->type == SHT_RELA) {
	//		fwrite(sectionHeaderGetExplicitRelocationData(sectionHeader), sectionHeader->size, 1, file);
        } else if (strcmp(sectionHeader->name, ".strtab") == 0) {
            fileSectionHeader->sh_info = 0;
            fileSectionHeader->sh_link = 0;
            fileSectionHeader->sh_flags = 0;
            fileSectionHeader->sh_addr = 0;
            fileSectionHeader->sh_addralign = 0;
            fileSectionHeader->sh_entsize = 0;
			fwrite(sectionHeaderGetRawData(sectionHeader), sectionHeader->size, 1, file);
		} else {
            fileSectionHeader->sh_info = 0;
			fwrite(sectionHeaderGetRawData(sectionHeader), sectionHeader->size, 1, file);
		}
    }

    //ecriture de la table des sections
    fileHeader.e_shoff = reverseEndian32(ftell(file));
    fwrite(fileSectionHeaderTable, fileSectionHeaderTableSize, 1, file);

    //eciture du header
    fseek(file, 0, SEEK_SET);
    fwrite(&fileHeader, sizeof(fileHeader), 1, file);
}
