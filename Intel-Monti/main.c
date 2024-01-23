#include "emulator.h"

int main()
{
	Emulator emulator = init_emulator();

	char program[] = {0x01, 0xFF, 0xFF, 0x02};

	execute_program(emulator, program, 4);

	free_emulator(emulator);

	return 0;
}
