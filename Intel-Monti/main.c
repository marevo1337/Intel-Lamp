#include "emulator.h"

int main()
{
	Emulator emulator = init_emulator();

	char program[] = {0x01, 0x03, 0x05, 0x03};

	execute_program(emulator, program, 4);

	free_emulator(emulator);

	return 0;
}
