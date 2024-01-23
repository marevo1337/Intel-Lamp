#pragma once

#include <stdio.h>
#include <stdint.h>

#include "../Memory/Register.h"
#include "../Memory/RAM.h"
#include "../Tools/BitOperation.h"

struct CPU
{
	// General-Purpose Registers
	Register8Bit A_Register;
	Register8Bit B_Register;
	Register8Bit C_Register;
	Register8Bit D_Register;
	Register8Bit E_Register;

	// General-Purpose Registers and Latched Address Register (HL)
	Register8Bit H_Register;
	Register8Bit L_Register;

	// Pointer and Index Registers
	Register16Bit stackPointer;
	Register16Bit programCounter;

	// Flag register
	FlagRegister flagRegister;
} typedef CPU;

CPU init_cpu();

void execute_cpu(CPU cpu, RAM* ramGateway);