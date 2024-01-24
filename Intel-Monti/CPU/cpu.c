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
                unsigned short DE = (cpu.D_Register.data << 8) | cpu.E_Register.data;
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
                cpu.programCounter.data++;

                break;
        }

        cpu.programCounter.data++;
    }
}
