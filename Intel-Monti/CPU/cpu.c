#include "cpu.h"

CPU init_cpu()
{
    CPU cpu;

    cpu.A_Register.data = 0;
    cpu.B_Register.data = 0;
    cpu.C_Register.data = 0;
    cpu.D_Register.data = 0;
    cpu.E_Register.data = 0;
    cpu.H_Register.data = 0;
    cpu.L_Register.data = 0;

    cpu.programCounter.data = 0;
    cpu.stackPointer.data = 0;

    return cpu;
}

void execute_cpu(CPU cpu, RAM* ramGateway)
{
    while (1)
    {
        char opCode = read_memory_ram(ramGateway, cpu.programCounter.data);
        
        switch (opCode)
        {
            // NOP
            case 0x00:
                break;
            // LXI B, D16
            case 0x01:
                cpu.programCounter.data++;
                cpu.B_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);
                
                cpu.programCounter.data++;
                cpu.C_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);
                break;
            // STAX B
            // After executing the STAX B instruction.
            // The content of register A is written to the memory at an address formed by the contents of registers B and C.
            case 0x02:
            {
                uint16_t targetAddress = (uint16_t) (cpu.B_Register.data << 8) | cpu.C_Register.data;
                write_memory_ram(ramGateway, targetAddress - 1, cpu.A_Register.data);
                break;
            }
            // INX B
            // Incrementing the value of the BC register pair.
            case 0x03:
            {
                uint16_t BC = (uint16_t) (cpu.B_Register.data << 8) | cpu.C_Register.data;

                BC++;
                cpu.B_Register.data = (BC >> 8) & 0xFF;
                cpu.C_Register.data = BC & 0xFF;

                break;
            }
            // INR B
            case 0x04:
                cpu.B_Register.data++;
                
                cpu.flagRegister.zeroFlag = (cpu.B_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.B_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.B_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.B_Register.data);

                break;
            // DCR B
            case 0x05:
                cpu.B_Register.data--;

                cpu.flagRegister.zeroFlag = (cpu.B_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.B_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.B_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.B_Register.data);

                break;
            // MVI B, D8
            case 0x06:
                cpu.programCounter.data++;
                cpu.B_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);

                break;
            // RLC
            // Thus, the RLC instruction performs a circular shift of bits to the left and stores the most significant bit in the carry flag 
            case 0x07:
            {
                cpu.flagRegister.carryFlag = (cpu.A_Register.data >> 7) & 1;
                cpu.A_Register.data = cpu.A_Register.data << 1;

                break;
            }
            // DAD B
            // DAD B instruction adds the contents of registers HL and BC, and the result is stored in register HL.
            // The carry flag (CY) is set if there is a carry from the most significant bit (high-order tetrad) during the addition operation
            // HL = HL + BC
            case 0x09:
            {
                uint16_t HL = (uint16_t) cpu.H_Register.data << 8 | cpu.L_Register.data;
                uint16_t BC = (uint16_t) cpu.B_Register.data << 8 | cpu.C_Register.data;

                short result = HL + BC;
                cpu.flagRegister.carryFlag = (result >> 16) & 0x01;

                cpu.H_Register.data = (unsigned char) (result >> 8);
                cpu.L_Register.data = (unsigned char) result;

                break;
            }
            // LDAX B
            // After the execution of this command, 
            // the value stored in the memory location whose address is formed by the contents of registers B and C will be loaded into the accumulator register A.
            case 0x0a:
            {
                uint16_t targetAddress = (cpu.B_Register.data << 8) | cpu.C_Register.data;
                cpu.A_Register.data = read_memory_ram(ramGateway, targetAddress - 1);
            
                break;
            }
            // DCX B
            // Decrement Double Register
            case 0x0b:
            {
                uint16_t BC = (cpu.B_Register.data << 8) | cpu.C_Register.data;
                BC--;

                cpu.B_Register.data = (BC >> 8) & 0xFF;
                cpu.C_Register.data = BC & 0xFF;

                break;
            }
            // INR C
            case 0x0c:
                cpu.C_Register.data++;

                cpu.flagRegister.zeroFlag = (cpu.C_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.C_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.C_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.C_Register.data);

                break;
            // DCR C
            case 0x0d:
                cpu.C_Register.data--;

                cpu.flagRegister.zeroFlag = (cpu.C_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.C_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.C_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.C_Register.data);

                break;
            // MVC C, D8
            case 0x0e:
                cpu.programCounter.data++;
                cpu.C_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);

                break;
            // RRC
            case 0x0f:
                cpu.flagRegister.carryFlag = (cpu.A_Register.data >> 7) & 1;
                cpu.A_Register.data = cpu.A_Register.data >> 1;

                break;
            // LXI D, D16
            case 0x11:
                cpu.programCounter.data++;
                cpu.D_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);

                cpu.programCounter.data++;
                cpu.E_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);

                break;
            // STAX D
            case 0x12:
            {
                uint16_t targetAddress = (uint16_t) (cpu.D_Register.data << 8) | cpu.E_Register.data;
                write_memory_ram(ramGateway, targetAddress - 1, cpu.A_Register.data);
                break;
            }
            // INX D
            case 0x13:
            {
                uint16_t DE = (uint16_t) (cpu.D_Register.data << 8) | cpu.E_Register.data;
                DE++;

                cpu.D_Register.data = (DE >> 8) & 0xFF;
                cpu.E_Register.data = DE & 0xFF;

                break;
            }
            // INR D
            case 0x14:
            {
                cpu.D_Register.data++;

                cpu.flagRegister.zeroFlag = (cpu.D_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.D_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.D_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.D_Register.data);

                break;
            }
            // DCR D
            case 0x15:
            {
                cpu.D_Register.data--;

                cpu.flagRegister.zeroFlag = (cpu.D_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.D_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.D_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.D_Register.data);

                break;
            }
            // 0x16 MVI D, D8
            // Move Immediate to Register D
            case 0x16:
                cpu.programCounter.data++;
                cpu.D_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);

                break;
            // 0x17 RAL
            // Rotate Accumulator Left through Carry
            case 0x17:
            {
                unsigned char prevÑarry = cpu.flagRegister.carryFlag;

                cpu.flagRegister.carryFlag = (cpu.A_Register.data >> 7) & 1;
                cpu.A_Register.data = (cpu.A_Register.data << 1) | prevÑarry;

                break;
            }
            // 0x19 DAD D
            // Double Add Register DE to HL
            case 0x19:
            {
                uint16_t HL = (uint16_t) cpu.H_Register.data << 8 | cpu.L_Register.data;
                uint16_t DE = (uint16_t) cpu.D_Register.data << 8 | cpu.E_Register.data;

                uint16_t result = HL + DE;
                cpu.flagRegister.carryFlag = (result >> 16) & 0x01;

                cpu.H_Register.data = (unsigned char) (result >> 8);
                cpu.L_Register.data = (unsigned char) result;

                break;
            }
            // 0x1a LDAX D
            // Load Accumulator from Memory through Register DE
            case 0x1a:
            {
                uint16_t targetAddress = (uint16_t) (cpu.D_Register.data << 8) | cpu.E_Register.data;
                cpu.A_Register.data = read_memory_ram(ramGateway, targetAddress - 1);

                break;
            }
            // 0x1b DCX D
            // Decrement Double Register DE
            case 0x1b:
            {
                uint16_t DE = (cpu.D_Register.data << 8) | cpu.E_Register.data;
                DE--;

                cpu.D_Register.data = (DE >> 8) & 0xFF;
                cpu.E_Register.data = DE & 0xFF;

                break;
            }
            // INR E
            case 0x1c:
                cpu.E_Register.data++;

                cpu.flagRegister.zeroFlag = (cpu.E_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.E_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.E_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.E_Register.data);

                break;

            // DCR E
            case 0x1d:
                cpu.E_Register.data--;

                cpu.flagRegister.zeroFlag = (cpu.E_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.E_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.E_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.E_Register.data);

                break;

            // MVI E, D8
            // Move immediate data (D8) to register E
            case 0x1e:
                cpu.programCounter.data++;
                cpu.E_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);

                break;

            // RAR
            // Rotate A right through carry
            case 0x1f:
            {
                unsigned char prevÑarry = cpu.flagRegister.carryFlag;

                cpu.flagRegister.carryFlag = cpu.A_Register.data & 1;
                cpu.A_Register.data >>= 1;
                cpu.A_Register.data |= (prevÑarry << 7);

                break;
            }
            // LXI H, D16
            // Load register pair HL with immediate data (D16)
            case 0x21:
                cpu.programCounter.data++;
                cpu.L_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);

                cpu.programCounter.data++;
                cpu.H_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);

                break;
            // SHLD adr
            // Store H and L registers direct to memory address specified by D16
            case 0x22:
            {
                cpu.programCounter.data++;
                short high = read_memory_ram(ramGateway, cpu.programCounter.data);

                cpu.programCounter.data++;
                short low = read_memory_ram(ramGateway, cpu.programCounter.data);

                uint16_t targetAddress = (high << 8) | low;

                write_memory_ram(ramGateway, targetAddress - 1, cpu.H_Register.data);
                write_memory_ram(ramGateway, targetAddress, cpu.L_Register.data);

                break;
            }
            // INX H
            case 0x23:
            {
                uint16_t HL = (uint16_t) cpu.H_Register.data << 8 | cpu.L_Register.data;
                HL++;

                cpu.H_Register.data = (HL >> 8) & 0xFF;
                cpu.L_Register.data = HL & 0xFF;
            }
            // INR H
            case 0x24:
            {
                cpu.H_Register.data++;

                // Update flags
                cpu.flagRegister.zeroFlag = (cpu.H_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.H_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.H_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.H_Register.data);

                break;
            }
            // DCR H
            case 0x25:
            {
                cpu.H_Register.data--;

                cpu.flagRegister.zeroFlag = (cpu.H_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.H_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.H_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.H_Register.data);

                break;
            }
            // MVI H, D8
            case 0x26:
                cpu.programCounter.data++;
                cpu.H_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);

                break;
            // DAA
            // Decimal Adjust Accumulator
            case 0x27:
            {
                char lowerNibble = cpu.A_Register.data & 0x0F;
                char higherNibble = (cpu.A_Register.data >> 4) & 0x0F;

                if (lowerNibble > 9 || cpu.flagRegister.auxiliaryCarry)
                {
                    cpu.A_Register.data += 6;
                    cpu.flagRegister.auxiliaryCarry = (lowerNibble + 6) & 0x10;
                }

                if (higherNibble > 9 || cpu.flagRegister.carryFlag)
                {
                    uint16_t sum = cpu.A_Register.data + 0x60;
                    cpu.A_Register.data = sum & 0xFF;

                    cpu.flagRegister.carryFlag = sum & 0x100;
                }

                cpu.flagRegister.zeroFlag = (cpu.A_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.A_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.A_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.A_Register.data);

                break;
            }
            // DAD H
            // Wrong - in the documenation http://www.emulator101.com/reference/8080-by-opcode.html
            // HL = HL + HI
            // I REGISTER??? WHAT THE FUCK?
            // Maybe this command try to HL = HL + HL?
            // I don't know!
            case 0x29:
            {
                uint16_t HL = (uint16_t) cpu.H_Register.data << 8 | cpu.L_Register.data;

                uint16_t result = (uint16_t) HL + HL;
                cpu.flagRegister.carryFlag = (result >> 16) & 0x01;

                cpu.H_Register.data = (result >> 8) & 0xFF;
                cpu.L_Register.data = result & 0xFF;

                break;
            }
            // LHLD adr
            case 0x2a:
            {
                cpu.programCounter.data++;
                short low = read_memory_ram(ramGateway, cpu.programCounter.data);

                cpu.programCounter.data++;
                short high = read_memory_ram(ramGateway, cpu.programCounter.data);

                uint16_t targetAddress = (high << 8) | low;

                cpu.L_Register.data = read_memory_ram(ramGateway, targetAddress - 1);
                cpu.H_Register.data = read_memory_ram(ramGateway, targetAddress);

                break;
            }
            // DCX H
            case 0x2b:
            {
                uint16_t HL = (cpu.H_Register.data << 8) | cpu.L_Register.data;
                HL--;

                cpu.H_Register.data = (HL >> 8) & 0xFF;
                cpu.L_Register.data = HL & 0xFF;

                break;
            }
            // INR L
            case 0x2c:
                cpu.L_Register.data++;

                cpu.flagRegister.zeroFlag = (cpu.L_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.L_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.L_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.L_Register.data);

                break;
            // DCR L
            case 0x2d:
                cpu.L_Register.data--;

                cpu.flagRegister.zeroFlag = (cpu.L_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.L_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.L_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.L_Register.data);

                break;
            // MVI L, D8
            case 0x2e:
                cpu.programCounter.data++;
                cpu.L_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);

                break;
            // CMA
            case 0x2f:
                cpu.A_Register.data = ~cpu.A_Register.data;

                break;
            // LXI SP, D16
            // Load 16-bit immediate data into Stack Pointer (SP)
            case 0x31:
            {
                cpu.programCounter.data++;
                short low = read_memory_ram(ramGateway, cpu.programCounter.data);

                cpu.programCounter.data++;
                short high = read_memory_ram(ramGateway, cpu.programCounter.data);

                uint16_t targetAddress = (high << 8) | low;

                short valueLow = read_memory_ram(ramGateway, targetAddress - 1);
                short valueHigh = read_memory_ram(ramGateway, targetAddress);

                cpu.programCounter.data = (valueHigh << 8) | valueLow;

                break;
            }
            // STA adr
            case 0x32:
            {
                cpu.programCounter.data++;
                short low = read_memory_ram(ramGateway, cpu.programCounter.data);

                cpu.programCounter.data++;
                short high = read_memory_ram(ramGateway, cpu.programCounter.data);

                uint16_t targetAddress = (high << 8) | low;

                write_memory_ram(ramGateway, targetAddress - 1, cpu.A_Register.data);

                break;
            }
            // INX SP
            case 0x33:
                cpu.programCounter.data++;

                break;
            // INR M
            // Increment the content of the memory cell whose address is specified by registers HL by 1
            case 0x34:
            {
                uint16_t targetAddress = (uint16_t)cpu.H_Register.data << 8 | cpu.L_Register.data;

                char value = read_memory_ram(ramGateway, targetAddress - 1);
                value++;
                write_memory_ram(ramGateway, targetAddress - 1, value);

                cpu.flagRegister.zeroFlag = (value == 0);
                cpu.flagRegister.signFlag = (value < 0);
                cpu.flagRegister.partyFlag = is_bits_even(value);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(value);

                break;
            }
            // DCR M
            // Decrement Memory
            // Decrement the content of the memory cell whose address is specified
            case 0x35:
            {
                uint16_t targetAddress = (uint16_t)cpu.H_Register.data << 8 | cpu.L_Register.data;

                char value = read_memory_ram(ramGateway, targetAddress - 1);
                value--;
                write_memory_ram(ramGateway, targetAddress, value);

                cpu.flagRegister.zeroFlag = (value == 0);
                cpu.flagRegister.signFlag = (value < 0);
                cpu.flagRegister.partyFlag = is_bits_even(value);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(value);

                break;
            }
            // MVI M, D8
            case 0x36:
            {
                uint16_t targetAddress = (uint16_t)cpu.H_Register.data << 8 | cpu.L_Register.data;
                cpu.programCounter.data++;

                char value = read_memory_ram(ramGateway, cpu.programCounter.data);

                write_memory_ram(ramGateway, targetAddress - 1, value);

                break;
            }
            // STC
            // Set the carry flag (CY) to 1
            case 0x37:
                cpu.flagRegister.carryFlag = 1;

                break;
            // DAD SP
            case 0x39:
            {
                uint16_t HL = (uint16_t) cpu.H_Register.data << 8 | cpu.L_Register.data;
                uint16_t stackPoitner = cpu.stackPointer.data;

                uint16_t result = HL + stackPoitner;
                cpu.flagRegister.carryFlag = (result >> 16) & 0x01;

                cpu.H_Register.data = (unsigned char) (result >> 8);
                cpu.L_Register.data = (unsigned char) result;

                break;
            }
            // DCX SP
            case 0x3b:
                cpu.stackPointer.data--;

                break;
            // INR A
            // Increment Register A
            case 0x3c:
                cpu.A_Register.data++;

                cpu.flagRegister.zeroFlag = (cpu.A_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.A_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.A_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.A_Register.data);

                break;
            // DCR A
            // Decrement Register A
            case 0x3d:
                cpu.A_Register.data++;

                cpu.flagRegister.zeroFlag = (cpu.A_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.A_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.A_Register.data);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(cpu.A_Register.data);

                break;
            // MVI A, D8
            // Move Immediate to Register A
            case 0x3e:
                cpu.programCounter.data++;
                cpu.A_Register.data = read_memory_ram(ramGateway, cpu.programCounter.data);

                break;
            // CMC
            // Complement Carry Flag
            case 0x3f:
                cpu.flagRegister.carryFlag = !cpu.flagRegister.carryFlag;

                break;
            // MOV B, B
            case 0x40:
                break;
            // MOV B, C
            case 0x41:
                cpu.B_Register.data = cpu.C_Register.data;

                break;
            // MOV B, D
            case 0x42:
                cpu.B_Register.data = cpu.D_Register.data;

                break;
            // MOV B, E
            case 0x43:
                cpu.B_Register.data = cpu.E_Register.data;

                break;
            // MOV B, H
            case 0x44:
                cpu.B_Register.data = cpu.H_Register.data;

                break;
            // MOV B, M
            // Move data from memory location addressed by HL to register B
            case 0x46:
            {
                uint16_t targetAddress = (uint16_t) cpu.H_Register.data << 8 | cpu.L_Register.data;
                cpu.B_Register.data = read_memory_ram(ramGateway, targetAddress - 1);

                break;
            }
            // MOV B, A
            // Move data from register A to register B
            case 0x47:
                cpu.B_Register.data = cpu.A_Register.data;
                break;
            // MOV C, B
            // Move data from register B to register C
            case 0x48:
                cpu.C_Register.data = cpu.B_Register.data;
                break;
            // MOV C, C
            // Move data from register C to register C (no operation)
            case 0x49:
                // No operation
                break;
            // MOV C, D
            // Move data from register D to register C
            case 0x4a:
                cpu.C_Register.data = cpu.D_Register.data;
                break;
            // MOV C, E
            // Move data from register E to register C
            case 0x4b:
                cpu.C_Register.data = cpu.E_Register.data;
                break;
            // MOV C, H
            // Move data from register H to register C
            case 0x4c:
                cpu.C_Register.data = cpu.H_Register.data;
                break;
            // MOV C, L
            // Move data from register L to register C
            case 0x4d:
                cpu.C_Register.data = cpu.L_Register.data;
                break;
            // MOV C, M
            // Move data from memory location specified by HL to register C
            case 0x4e:
            {
                uint16_t targetAddress = (uint16_t) cpu.H_Register.data << 8 | cpu.L_Register.data;

                cpu.C_Register.data = read_memory_ram(ramGateway, targetAddress - 1);
                break;
            }
            // MOV C, A
            // Move data from register A to register C
            case 0x4f:
                cpu.C_Register.data = cpu.A_Register.data;
                break;
            // MOV D, B
            // Move data from register B to register D
            case 0x50:
                cpu.D_Register.data = cpu.B_Register.data;
                break;
            // MOV D, C
            // Move data from register C to register D
            case 0x51:
                cpu.D_Register.data = cpu.C_Register.data;
                break;
            // MOV D, D
            // Move data from register D to register D (no operation)
            case 0x52:
                // No operation
                break;
            // MOV D, E
            // Move data from register E to register D
            case 0x53:
                cpu.D_Register.data = cpu.E_Register.data;
                break;
            // MOV D, H
            // Move data from register H to register D
            case 0x54:
                cpu.D_Register.data = cpu.H_Register.data;
                break;
            // MOV D, L
            // Move data from register L to register D
            case 0x55:
                cpu.D_Register.data = cpu.L_Register.data;
                break;
            // MOV D, M
            // Move data from memory location specified by HL to register D
            case 0x56:
            {
                uint16_t targetAddress = (uint16_t)cpu.H_Register.data << 8 | cpu.L_Register.data;

                cpu.D_Register.data = read_memory_ram(ramGateway, targetAddress - 1);
                break;
            }
            // MOV D, A
            // Move data from register A to register D
            case 0x57:
                cpu.D_Register.data = cpu.A_Register.data;
                break;
            // MOV E, B
            // Move data from register B to register E
            case 0x58:
                cpu.E_Register.data = cpu.B_Register.data;
                break;
            // MOV E, C
            // Move data from register C to register E
            case 0x59:
                cpu.E_Register.data = cpu.C_Register.data;
                break;
            // MOV E, D
            // Move data from register D to register E
            case 0x5a:
                cpu.E_Register.data = cpu.D_Register.data;
                break;
            // MOV E, E
            // Move data from register E to register E (no operation)
            case 0x5b:
                // No operation
                break;
            // MOV E, H
            // Move data from register H to register E
            case 0x5c:
                cpu.E_Register.data = cpu.H_Register.data;
                break;
            // MOV E, L
            // Move data from register L to register E
            case 0x5d:
                cpu.E_Register.data = cpu.L_Register.data;
                break;
            // MOV E, M
            // Move data from memory location specified by HL to register E
            case 0x5e:
            {
                uint16_t targetAddress = (uint16_t)cpu.H_Register.data << 8 | cpu.L_Register.data;

                cpu.E_Register.data = read_memory_ram(ramGateway, targetAddress - 1);
                break;
            }
            // MOV E, A
            // Move data from register A to register E
            case 0x5f:
                cpu.E_Register.data = cpu.A_Register.data;
                break;
            // MOV H, B
            // Move data from register B to register H
            case 0x60:
                cpu.H_Register.data = cpu.B_Register.data;
                break;
            // MOV H, C
            // Move data from register C to register H
            case 0x61:
                cpu.H_Register.data = cpu.C_Register.data;
                break;
            // MOV H, D
            // Move data from register D to register H
            case 0x62:
                cpu.H_Register.data = cpu.D_Register.data;
                break;
            // MOV H, E
            // Move data from register E to register H
            case 0x63:
                cpu.H_Register.data = cpu.E_Register.data;
                break;
            // MOV H, H
            // Move data from register H to register H (no operation)
            case 0x64:
                // No operation
                break;
            // MOV H, L
            // Move data from register L to register H
            case 0x65:
                cpu.H_Register.data = cpu.L_Register.data;
                break;
            // MOV H, M
            // Move data from memory location specified by HL to register H
            case 0x66:
            {
                uint16_t targetAddress = (uint16_t) cpu.H_Register.data << 8 | cpu.L_Register.data;

                cpu.H_Register.data = read_memory_ram(ramGateway, targetAddress - 1);
                break;
            }
            // MOV H, A
            // Move data from register A to register H
            case 0x67:
                cpu.H_Register.data = cpu.A_Register.data;
                break;
            // MOV L, B
            // Move data from register B to register L
            case 0x68:
                cpu.L_Register.data = cpu.B_Register.data;
                break;
            // MOV L, C
            // Move data from register C to register L
            case 0x69:
                cpu.L_Register.data = cpu.C_Register.data;
                break;
            // MOV L, D
            // Move data from register D to register L
            case 0x6a:
                cpu.L_Register.data = cpu.D_Register.data;
                break;
            // MOV L, E
            // Move data from register E to register L
            case 0x6b:
                cpu.L_Register.data = cpu.E_Register.data;
                break;
            // MOV L, H
            // Move data from register H to register L
            case 0x6c:
                cpu.L_Register.data = cpu.H_Register.data;
                break;
            // MOV L, L
            // Move data from register L to register L (no operation)
            case 0x6d:
                // No operation
                break;
            // MOV L, M
            case 0x6e:
            {
                uint16_t targetAddress = (uint16_t) (cpu.H_Register.data << 8) | cpu.L_Register.data;
                cpu.L_Register.data = read_memory_ram(ramGateway, targetAddress - 1);

                break;
            }
            // MOV L, A
            case 0x6f:
            {
                cpu.L_Register.data = cpu.A_Register.data;

                break;
            }
            // MOV M, B
            case 0x70:
            {
                uint16_t targetAddress = (uint16_t) (cpu.H_Register.data << 8) | cpu.L_Register.data;
                write_memory_ram(ramGateway, targetAddress - 1, cpu.B_Register.data);

                break;
            }
            // MOV M, C
            case 0x71:
            {
                uint16_t targetAddress = (uint16_t)(cpu.H_Register.data << 8) | cpu.L_Register.data;
                write_memory_ram(ramGateway, targetAddress - 1, cpu.C_Register.data);

                break;
            }
            // MOV M, D
            case 0x72:
            {
                uint16_t targetAddress = (uint16_t)(cpu.H_Register.data << 8) | cpu.L_Register.data;
                write_memory_ram(ramGateway, targetAddress - 1, cpu.D_Register.data);

                break;
            }
            // MOV M, E
            case 0x73:
            {
                uint16_t targetAddress = (uint16_t) (cpu.H_Register.data << 8) | cpu.L_Register.data;
                write_memory_ram(ramGateway, targetAddress - 1, cpu.E_Register.data);

                break;
            }
            // MOV M, H
            case 0x74:
            {
                uint16_t targetAddress = (uint16_t) (cpu.H_Register.data << 8) | cpu.L_Register.data;
                write_memory_ram(ramGateway, targetAddress - 1, cpu.H_Register.data);

                break;
            }
            // MOV M, L
            case 0x75:
            {
                uint16_t targetAddress = (uint16_t) (cpu.H_Register.data << 8) | cpu.L_Register.data;
                write_memory_ram(ramGateway, targetAddress - 1, cpu.H_Register.data);

                break;
            }
            // HLT
            // Halt mode
            case 0x76:
                break;
            // MOV M, A
            case 0x77:
            {
                uint16_t targetAddress = (uint16_t) (cpu.H_Register.data << 8) | cpu.L_Register.data;
                write_memory_ram(ramGateway, targetAddress - 1, cpu.A_Register.data);

                break;
            }
            // MOV A, B
            case 0x78:
                cpu.A_Register.data = cpu.B_Register.data;
                break;
            // MOV A,C
            // Move content of register C to A
            case 0x79:
                cpu.A_Register.data = cpu.C_Register.data;
                break;
            // MOV A,D
            // Move content of register D to A
            case 0x7a:
                cpu.A_Register.data = cpu.D_Register.data;
                break;
            // MOV A,E
            // Move content of register E to A
            case 0x7b:
                cpu.A_Register.data = cpu.E_Register.data;
                break;
            // MOV A,H
            // Move content of register H to A
            case 0x7c:
                cpu.A_Register.data = cpu.H_Register.data;
                break;
            // MOV A,L
            // Move content of register L to A
            case 0x7d:
                cpu.A_Register.data = cpu.L_Register.data;
                break;
            // MOV A, M
            case 0x7e:
            {
                uint16_t targetAddress = (uint16_t) (cpu.H_Register.data << 8) | cpu.L_Register.data;
                cpu.A_Register.data = read_memory_ram(ramGateway, targetAddress - 1);

                break;
            }
            // MOV A, A
            case 0x7f:
                break;
            // ADD B
            case 0x80:
            {
                unsigned char sum = cpu.A_Register.data + cpu.B_Register.data;

                cpu.flagRegister.zeroFlag = (sum == 0);
                cpu.flagRegister.signFlag = (sum < 0);
                cpu.flagRegister.partyFlag = is_bits_even(sum & 0xFF);
                cpu.flagRegister.carryFlag = (sum > 0xFF);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(sum);

                cpu.A_Register.data = sum;

                break;
            }
            // ADD C
            case 0x81:
            {
                unsigned char sum = cpu.A_Register.data + cpu.C_Register.data;

                cpu.flagRegister.zeroFlag = (sum == 0);
                cpu.flagRegister.signFlag = (sum < 0);
                cpu.flagRegister.partyFlag = is_bits_even(sum & 0xFF);
                cpu.flagRegister.carryFlag = (sum > 0xFF);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(sum);

                cpu.A_Register.data = sum;

                break;
            }
            // ADD D
            case 0x82:
            {
                unsigned char sum = cpu.A_Register.data + cpu.D_Register.data;

                cpu.flagRegister.zeroFlag = (sum == 0);
                cpu.flagRegister.signFlag = (sum < 0);
                cpu.flagRegister.partyFlag = is_bits_even(sum & 0xFF);
                cpu.flagRegister.carryFlag = (sum > 0xFF);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(sum);

                cpu.A_Register.data = sum;

                break;
            }
            // ADD E
            case 0x83:
            {
                unsigned char sum = cpu.A_Register.data + cpu.E_Register.data;

                cpu.flagRegister.zeroFlag = (sum == 0);
                cpu.flagRegister.signFlag = (sum < 0);
                cpu.flagRegister.partyFlag = is_bits_even(sum & 0xFF);
                cpu.flagRegister.carryFlag = (sum > 0xFF);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(sum);

                cpu.A_Register.data = sum;

                break;
            }
            // ADD H
            case 0x84:
            {
                char sum = cpu.A_Register.data + cpu.H_Register.data;

                cpu.flagRegister.zeroFlag = (sum == 0);
                cpu.flagRegister.signFlag = (sum < 0);
                cpu.flagRegister.partyFlag = is_bits_even(sum & 0xFF);
                cpu.flagRegister.carryFlag = (sum > 0xFF);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(sum);

                cpu.A_Register.data = sum;

                break;
            }
            // ADD L
            case 0x85:
            {
                char sum = cpu.A_Register.data + cpu.L_Register.data;

                cpu.flagRegister.zeroFlag = (sum == 0);
                cpu.flagRegister.signFlag = (sum < 0);
                cpu.flagRegister.partyFlag = is_bits_even(sum & 0xFF);
                cpu.flagRegister.carryFlag = (sum > 0xFF);
                cpu.flagRegister.auxiliaryCarry = is_auxiliary_carry_set(sum);

                cpu.A_Register.data = sum;

                break;
            }
        }

        cpu.programCounter.data++;
    }
}
