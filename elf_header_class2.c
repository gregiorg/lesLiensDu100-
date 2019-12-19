Header* headerFromFile(FILE* file) {
    ElfHeader elfHeader;
    fseek(file, 0, SEEK_SET);
    fread(&elfHeader, sizeof (elfHeader), 1, file);

    Header* header = malloc(sizeof(Header));

    setHeader(elfHeader, header);

    header->sectionHeaderTable = sectionHeaderTableFromFile(
        file, 
        reverseEndian32(elfHeader.offset), 
        header, 
        reverseEndian16(elfHeader.shnum)
    );

    return header;
}

SecionHeaderTable* sectionHeaderTableFromFile(FILE* file, int offset, Header* header, int nbSections, int shxstr) {
    ElfSecHeader* elfSecHeaders;
    fseek(file, offset, SEEK_SET);
    fread(&elfSecHeaders, sizeof (ElfSecHeader), nbSections, file);

    SectionHeaderTable* sectionHeaderTable = malloc(sizeof(SectionHeaderTable));

    sectionHeaderTable->header = header;
    sectionHeaderTable->nbSections = nbSections;
    sectionHeaderTable->sections = malloc(sizeof(void*) * nbSections);

    for (int i = 0; i < nbSections; i++) {
        setSectionHeaderTable(sectionHeaderTable, elfSecHeaders);
    }

    char* stringTable = (char*) sectionHeaderTable->sections[shxstr]->data;

    for (int i = 0; i < nbSections; i++) {
        currentSectionHeader->name = stringTable + reverseEndian32(currentElfSecHeader.name);
    }

    return sectionHeaderTable;
}

void setHeader(ElfHeader elfHeader, Header* header) {
	header->indentClass = elfHeader.indentClass;
    header->indentData = elfHeader.indentData;
    header->indentVersion = elfHeader.indentVersion;
    header->type = reverseEndian16(elfHeader.type);
    header->machine = reverseEndian16(elfHeader.machine);
    header->version = reverseEndian32(elfHeader.version);
    header->entry = reverseEndian32(elfHeader.entry);
    header->flags = reverseEndian32(elfHeader.flags);
}

void setSectionHeaderTable(SectionHeaderTable* sectionHeaderTable, ElfSecHeader* elfSecHeaders) {
	currentSectionHeader = malloc(sizeof(SectionHeader));
	
	ElfSecHeader currentElfSecHeader = elfSecHeaders[i];
	SectionHeader* currentSectionHeader = sectionHeaderTable->sections[i];

	uint32_t size = reverseEndian32(currentElfSecHeader.size);

	currentSectionHeader->indexTable = i;
	currentSectionHeader->type = reverseEndian32(currentElfSecHeader.type);
	currentSectionHeader->flags = reverseEndian32(currentElfSecHeader.flags);
	currentSectionHeader->addr = reverseEndian32(currentElfSecHeader.addr);
	currentSectionHeader->link = reverseEndian32(currentElfSecHeader.link);
	currentSectionHeader->info = reverseEndian32(currentElfSecHeader.info);
	currentSectionHeader->addrAlign = reverseEndian32(currentElfSecHeader.addrAlign);
	currentSectionHeader->entSize = reverseEndian32(currentElfSecHeader.entSize);

	currentSectionHeader->data = malloc(size)
	
	fseek(file, reverseEndian32(currentElfSecHeader.offset), SEEK_SET);
	fread(currentSectionHeader->data, size, 1, file);
}