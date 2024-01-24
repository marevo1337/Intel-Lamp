#pragma once

#include <stdint.h>

struct Register8Bit
{
	char data;
} typedef Register8Bit;

struct Register16Bit
{
	uint16_t data;
} typedef Register16Bit;

struct FlagRegister
{
	int zeroFlag;
	int signFlag;
	int partyFlag;
	int auxiliaryCarry;
	char carryFlag;
} typedef FlagRegister;