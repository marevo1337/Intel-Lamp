#pragma once

#include <stdio.h>
#include <stdint.h>

#include "../Memory/Register.h"
#include "../Memory/RAM.h"

#define ZERO_FLAG 1
#define SIGN_FLAG 2
#define PARTY_FLAG 3
#define AUXILIARY_CARRY 4

struct CPU
{
	// General-Purpose Registers
	Register A_Register;
	Register B_Register;
	Register C_Register;
	Register D_Register;
	Register E_Register;

	// General-Purpose Registers and Latched Address Register (HL)
	Register H_Register;
	Register L_Register;

	// Pointer and Index Registers
	Register stackPointer;
	Register programCounter;

	// Flag register
	Register flagRegister;
} typedef CPU;

CPU init_cpu();

void execute_cpu(CPU cpu, RAM* ramGateway);