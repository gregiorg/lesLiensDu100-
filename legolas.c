#include "elf_header_class.h"

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

			break;
		}

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

Header* headerFromFile(FILE* file) {
    ElfHeader elfHeader;
    fseek(file, 0, SEEK_SET);
    fread(&elfHeader, sizeof(elfHeader), 1, file);

    Header* header = malloc(sizeof(Header));

    setHeader(elfHeader, header);

    ElfSecHeader* elfSecHeaders = malloc(sizeof(ElfSecHeader) * header->nbSections);
    fseek(file, reverseEndian32(elfHeader.shoff), SEEK_SET);
    fread(elfSecHeaders, sizeof(ElfSecHeader), header->nbSections, file);

    header->sections = malloc(sizeof(void*) * header->nbSections);

    for (int i = 0; i < header->nbSections; i++) {
        setSectionHeader(header, elfSecHeaders, i, file);
    }

	int shstrndx = reverseEndian16(elfHeader.shstrndx);
    char* stringTable = (char*) header->sections[shstrndx]->rawData;

    for (int i = 0; i < header->nbSections; i++) {
		SectionHeader* currentSectionHeader = header->sections[i];
		ElfSecHeader currentElfSecHeader = elfSecHeaders[i];
        currentSectionHeader->name = stringTable + reverseEndian32(currentElfSecHeader.name);
    }

	return header;
}

void legolasWriteToFile(Header* header, FILE* file) {
    //creation de la table des chaines
    SectionHeader* stringTableSectionHeader = malloc(sizeof(SectionHeader));
    stringTableSectionHeader->name = ".str_table";
    stringTableSectionHeader->type = SHT_STRTAB;
    stringTableSectionHeader->size = 0;
    headerAddSection(stringTableSectionHeader);

	//creation du header
    Elf32_Ehdr fileHeader;

	fileHeader.e_indent[EI_MAG0] = ELFMAG0;
	fileHeader.e_indent[EI_MAG1] = ELFMAG1;
	fileHeader.e_indent[EI_MAG2] = ELFMAG2;
	fileHeader.e_indent[EI_MAG3] = ELFMAG3;
	fileHeader.e_indent[EI_CLASS] = header->indentClass;
    fileHeader.e_indent[EI_DATA] = header->indentData ;
    fileHeader.e_indent[EI_VERSION] = header->indentVersion;
    fileHeader.e_indent[EI_OSABI] = header->indentOSABI;
    fileHeader.e_indent[EI_ABIVERSION] = header->indentABIVersion;
    fileHeader.e_type = reverseEndian16(header->type);
    fileHeader.e_machine = reverseEndian16(header->machine);
    fileHeader.e_version = reverseEndian32(header->version);
    fileHeader.e_entry = reverseEndian32(header->entry);
    fileHeader.e_flags = reverseEndian32(header->flags);
	fileHeader.e_ehsize = reverseEndian16(sizeof(fileHeader));
	fileHeader.e_shnum = reverseEndian16(header->shnum);
    fileHeader.e_shstrndx = reverseEndian16(headerGetIndexOfSectionHeader(header, stringTableSectionHeader));

	//on laisse de la place dans le fichier pour le header
	fseek(file, sizeof(fileHeader), SEEK_SET);

	//creation de la table des sections
	Elf32_Shdr* fileSectionHeaderTable = malloc(sizeof(Elf32_Shdr) * header->shnum);

    //remplissage de la table des chaines
	for (int i = 0; i < header->shnum; i++) {
        SectionHeader* sectionHeader = header->sectionHeaderTable[i];
        Elf32_Shdr* fileSectionHeader = &(fileSectionHeaderTable[i]);

        int name = StringTableAddString(stringTableSectionHeader, sectionHeader->name)
        fileSectionHeader->sh_name = reverseEndian32(name);
    }

	//ecriture des sections dans le fichier et memorisation des offsets
	for (int i = 0; i < header->shnum; i++) {
        SectionHeader* sectionHeader = header->sectionHeaderTable[i];
        Elf32_Shdr* fileSectionHeader = &(fileSectionHeaderTable[i]);

		fileSectionHeader->sh_type = reverseEndian32(sectionHeader->type);
		fileSectionHeader->sh_flags = reverseEndian32(sectionHeader->flags);
		fileSectionHeader->sh_addr = reverseEndian32(sectionHeader->addr);
		fileSectionHeader->sh_link = reverseEndian32(sectionHeader->link);
		fileSectionHeader->sh_info = reverseEndian32(sectionHeader->info);
		fileSectionHeader->sh_addralign = reverseEndian32(sectionHeader->addralign);

        fileSectionHeader->sh_size = reverseEndian32(sectionHeaderGetSize(sectionHeader));
        fileSectionHeader->sh_entsize = reverseEndian32(sectionHeaderGetEntSize(sectionHeader));

		fileSectionHeader->sh_offset = reverseEndian32(ftell(file));
        fwrite(sectionHeaderGetData(sectionHeader), fileSectionHeader->sh_size, 1, file);
	}

	//ecriture de la table des sections
	fileHeader.e_shoff = reverseEndian32(ftell(file));
	fwrite(fileSectionHeaderTable, sizeof(*fileSectionHeaderTable), 1, file);

	//eciture du header
	fseek(file, 0, SEEK_SET);
	fwrite(&fileHeader, sizeof(fileHeader), 1, file);
}
