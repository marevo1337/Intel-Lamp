#include "RAM.h"

// CS6011 warning is ambiguous
#pragma warning(disable : 6011)

RAM* init_ram()
{
    RAM_MemoryBlock* memoryBlocks = (RAM_MemoryBlock*) malloc(sizeof(RAM_MemoryBlock) * RAM_MEMORY_SIZE);

    for (int i = 0; i < RAM_MEMORY_SIZE; i++)
    {
        memoryBlocks[i].memoryAddress = i + 1;
        memoryBlocks[i].rawByte = 0;
    }

    RAM* ram = malloc(sizeof(RAM));
    ram->blocks = memoryBlocks;

    return ram;
}

char read_memory_ram(RAM* ramPointer, unsigned short offset)
{
    if (offset < 0 || offset > RAM_MEMORY_SIZE)
    {
        return NULL;
    }

    return ramPointer->blocks[offset].rawByte;
}

void write_memory_ram(RAM* ramPointer, unsigned short offset, char byte)
{
    if (offset < 0 || offset > RAM_MEMORY_SIZE)
    {
        return;
    }

    ramPointer->blocks[offset].rawByte = byte;
}

void free_ram(RAM* ramPointer)
{
    free(ramPointer->blocks);
    free(ramPointer);
}
