#include "emulator.h"

Emulator init_emulator()
{
    Emulator emulator;

    emulator.ram = init_ram();

    return emulator;
}

void free_emulator(Emulator emulator)
{
    free_ram(emulator.ram);
}

void execute_program(Emulator emulator, char* opCodesBuffer, int opCodesBufferSize, int start)
{
    int programStart = start;

    if (opCodesBufferSize + programStart >= RAM_MEMORY_SIZE)
    {
        printf("%s\n", "[ERROR] Out of range memory size");
        return;
    }

    // Loading program opcodes into virtual RAM
    for (int i = programStart; i < opCodesBufferSize; i++)
    {
        write_memory_ram(emulator.ram, i, opCodesBuffer[i]);
    }

    // Moving the PC register to the beginning of the program
    emulator.cpu.programCounter.data = programStart;

    execute_cpu(emulator.cpu, emulator.ram);
}
