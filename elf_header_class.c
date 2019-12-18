Header* headerFromFile(FILE* file) {
    ElfHeader elfHeader;
    fseek(file, 0, SEEK_SET);
    fread(&elfHeader, sizeof (elfHeader), 1, file);

    Header* header = malloc(sizeof(Header));

    header->indentClass = elfHeader.indentClass;
    header->indentData = elfHeader.indentData;
    header->indentVersion = elfHeader.indentVersion;
    header->type = reverseEndian16(elfHeader.type);
    header->machine = reverseEndian16(elfHeader.machine);
    header->version = reverseEndian32(elfHeader.version);
    header->entry = reverseEndian32(elfHeader.entry);
    header->flags = reverseEndian32(elfHeader.flags);

    header->sectionHeaderTable = sectionHeaderTableFromFile(
        file, 
        reverseEndian32(elfHeader.offset), 
        header, 
        reverseEndian16(elfHeader.shnum)
    );

    return header;
}

SecionHeaderTable* sectionHeaderTableFromFile(FILE* file, int offset, Header* header, int nbSections) {
    ElfSecHeader* elfSecHeaders;
    fseek(file, offset, SEEK_SET);
    fread(&elfSecHeaders, sizeof (ElfSecHeader), nbSections, file);

    SectionHeaderTable* sectionHeaderTable = malloc(sizeof(SectionHeaderTable));

    sectionHeaderTable->header = header;
    sectionHeaderTable->nbSections = nbSections;
    sectionHeaderTable->sections = malloc(sizeof(void*) * nbSections);

    for (int i = 0; i < nbSections; i++) {
        sectionHeaderTable->sections[i] = malloc(sizeof(SectionHeader));

        sectionHeaderTable->sections[i]->indexTable = i;
        sectionHeaderTable->sections[i]->type = reverseEndian32(elfSecHeaders[i].type);
        sectionHeaderTable->sections[i]->flags = reverseEndian32(elfSecHeaders[i].flags);
        sectionHeaderTable->sections[i]->addr = reverseEndian32(elfSecHeaders[i].addr);
        sectionHeaderTable->sections[i]->link = reverseEndian32(elfSecHeaders[i].link);
        sectionHeaderTable->sections[i]->info = reverseEndian32(elfSecHeaders[i].info);
        sectionHeaderTable->sections[i]->addrAlign = reverseEndian32(elfSecHeaders[i].addrAlign);
        sectionHeaderTable->sections[i]->entSize = reverseEndian32(elfSecHeaders[i].entSize);

        sectionHeaderTable->sections[i]->data = malloc(reverseEndian32(elfSecHeaders[i].size))
        fseek(file, reverseEndian32(elfSecHeaders[i].offset), SEEK_SET);
        fread(sectionHeaderTable->sections[i]->data, reverseEndian32(elfSecHeaders[i].size), 1, file);
    }

    char* sectionHeaderStringTable;

    for (int i = 0; i < nbSections; i++) {
        if (sectionHeaderTable
    }

    return sectionHeaderTable;
}
