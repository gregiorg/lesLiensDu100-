#include <stdio.h>

typedef struct
{
	uint32_t name;
	uint32_t type;
	uint32_t flags;
	uint32_t addr;
	uint32_t offset;
	uint32_t size;
	uint32_t link;
	uint32_t info;
	uint32_t addralign;
	uint32_t entsize;
} Elf_SecHeader;

int main (int argc, char** argv)
{
	FILE f = fopen(argv[1], "r");

	Elf_SecHeader elf_secHeader;

	//on récupèrera tailleHeader de l'étape 1 (720 = taille du header de exemple 0)

	const uint32_t tailleHeader = 720;

	//on récupèrera nbSections de l'étape 1 (15 = nb sections de exemple 0)

	const uint32_t nbSections = 15;

	fseek(f, tailleHeader, SEEK_SET);

	for (int i=0; i < nbSections; i++)
	{
		fread(&elf_secHeader, sizeof (elf_secHeader), 1, f);

		afficherHeader(elf_secHeader);

		const uint32_t tailleSection_i = Elf_SecHeader.size;

		fseek(f, tailleSection_i, SEEK_CUR);
	}
}

void afficherHeader(Elf_SecHeader elf_secHeader)
{
	//TODO
}

