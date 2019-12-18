typedef struct {
    uint8_t indentClass;
    uint8_t indentData;
    uint8_t indentVersion;
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint32_t entry;
    uint32_t flags;
    SectionHeaderTable* sectionHeaderTable;
} Header;

Header* headerFromFile(FILE* file);
void headerWriteToFile(Header*, FILE* file);
//char* headerGetString(int index);

typedef struct {
    SectionHeader** sections;
    int nbSections;
    Header* header;
} SectionHeaderTable;

int sectionHeaderTableGetSize(SectionHeaderTable*);
SectionHeader** sectionHeaderTableGetSectionsOfType(SectionHeaderTable*, uint32_t, int*);
SectionHeader** sectionHeaderTableGetSectionsWithName(SectionHeaderTable*, char*, int*);
//SectionHeader* sectionHeaderTableGetSectionOfType(SectionHeaderTable*, uint32_t);
//SectionHeader* sectionHeaderTableGetSectionWithName(SectionHeaderTable*, char*);

typedef struct {
	unsigned int indexTable;
	char* name;
	uint32_t type;
	uint32_t flags;
	uint32_t addr;
	uint32_t link;
	uint32_t info;
	uint32_t addrAlign;
	uint32_t entSize;
    void* data;
} SectionHeader;

const char* sectionHeaderGetNameString(SectionHeader*);
const char* sectionHeaderGetTypeString(SectionHeader*);
int sectionHeaderGetSize(SectionHeader*);
RelocationTable* sectionHeaderGetRelocationTable(SectionHeader*);
SymboleTable* sectionHeaderGetSymboleTable(SectionHeader*);

typedef struct {
    SymboleTableEntry**;
    int nbrEntry;
} SymboleTable;

typedef struct {
    char* name;
    uint32_t value;
    uint32_t size;
    char bind;
    char type;
    char other;
    SectionHeader* sectionHeader;
} SymboleTableEntry;

typedef struct {
    RelocationTableEntry**;
    int nbrEntry;
} RelocationTable;

typedef struct {
    SymboleTableEntry* sym;
    char type;
} RelocationTableEntry;