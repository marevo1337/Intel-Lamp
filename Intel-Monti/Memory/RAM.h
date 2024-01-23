#pragma once

#include <stdlib.h>

// The Intel 8080 processor had an address space for RAM of up to 64 KB.
// Corresponding to addresses ranging from 0x0000 to 0xFFFF
#define RAM_MEMORY_SIZE 65535

// Representation of a block of random-access memory.
// The Intel 8080 processor stored 1 byte of information in a single memory cell
struct RAM_MemoryBlock
{
	unsigned short memoryAddress;
	char rawByte;
} typedef RAM_MemoryBlock;

struct RAM
{
	RAM_MemoryBlock* blocks;
} typedef RAM;

RAM* init_ram();

char read_memory_ram(RAM* ramPointer, unsigned short offset);
void write_memory_ram(RAM* ramPointer, unsigned short offset, char byte);

void free_ram(RAM* ramPointer);
