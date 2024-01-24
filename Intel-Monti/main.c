#include <stdlib.h>

#include "emulator.h"

int main(int argv, char* argc)
{
	if (argv == 1)
	{
		printf("%s", "[ERROR] Need executable file");
		return 1;
	}

	FILE* file = NULL;
	errno_t errno_file = fopen_s(file, argc[1], "rb");
	if (errno_file != 0)
	{
		printf("%s", "[ERROR] Can not open file");
		return 1;
	}

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);

	rewind(file);

	char* opCodesBuffer = (char*) malloc(file_size);
	if (opCodesBuffer == NULL)
	{
		printf("%s", "[ERROR] Can not open file");
		return 1;
	}

	size_t read_size = fread(opCodesBuffer, 1, file_size, file);

	Emulator emulator = init_emulator();
	execute_program(emulator, opCodesBuffer, read_size, 0);

	free_emulator(emulator);
	free(opCodesBuffer);
	fclose(file);

	return 0;
}
