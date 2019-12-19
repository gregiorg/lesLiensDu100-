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

	currentSectionHeader->data = malloc(currentSectionHeader->size);
	fseek(file, reverseEndian32(currentElfSecHeader.offset), SEEK_SET);
	fread(currentSectionHeader->data, currentSectionHeader->size, 1, file);

	header->sections[i] = currentSectionHeader;
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
    char* stringTable = (char*) header->sections[shstrndx]->data;

    for (int i = 0; i < header->nbSections; i++) {
		SectionHeader* currentSectionHeader = header->sections[i];
		ElfSecHeader currentElfSecHeader = elfSecHeaders[i];
        currentSectionHeader->name = stringTable + reverseEndian32(currentElfSecHeader.name);
    }

	return header;
}

void headerWriteToFile(Header* header, FILE* file) {
	ElfHeader elfHeader;

	elfHeader.indentMagicNumber[0] = 0x7F;
	elfHeader.indentMagicNumber[1] = 'E';
	elfHeader.indentMagicNumber[2] = 'L';
	elfHeader.indentMagicNumber[3] = 'F';
	elfHeader.indentClass = header->indentClass;
    elfHeader.indentData = header->indentData ;
    elfHeader.indentVersion = header->indentVersion;
    elfHeader.type = reverseEndian16(header->type);
    elfHeader.machine = reverseEndian16(header->machine);
    elfHeader.version = reverseEndian32(header->version);
    elfHeader.entry = reverseEndian32(header->entry);
    elfHeader.flags = reverseEndian32(header->flags);
	elfHeader.shnum = reverseEndian16(header->nbSections);
	elfHeader.shoff = 1000;

	fseek(file, 0, SEEK_SET);
	fwrite(&elfHeader, sizeof(ElfHeader), 1, file);
}
