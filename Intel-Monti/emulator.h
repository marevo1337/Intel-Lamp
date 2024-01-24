#include <stdio.h>

#include "CPU/cpu.h"
#include "Memory/RAM.h"

struct Emulator
{
	CPU cpu;
	RAM* ram;
} typedef Emulator;

Emulator init_emulator();
void free_emulator(Emulator emulator);

void execute_program(Emulator emulator, char* opCodesBuffer, int opCodesBufferSize, int start);