#pragma once

struct Register8Bit
{
	char data;
} typedef Register8Bit;

struct Register16Bit
{
	short data;
} typedef Register16Bit;

struct FlagRegister
{
	int zeroFlag;
	int signFlag;
	int partyFlag;
	int auxliaryCarry;
} typedef FlagRegister;