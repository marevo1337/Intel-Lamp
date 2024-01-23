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

        printf("%x\n", opCode);
        
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
                unsigned short targetAddress = ((unsigned short)cpu.B_Register.data << 8) | cpu.C_Register.data;
                write_memory_ram(ramGateway, targetAddress - 1, cpu.A_Register.data);
                break;
            }
            // INX B
            // Incrementing the value of the BC register pair.
            case 0x03:
            {
                unsigned short BC = (cpu.B_Register.data << 8) | cpu.C_Register.data;

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
                cpu.flagRegister.auxliaryCarry = is_auxiliary_carry_set(cpu.B_Register.data);

                break;
            // DCR B
            case 0x05:
                cpu.B_Register.data--;

                cpu.flagRegister.zeroFlag = (cpu.B_Register.data == 0);
                cpu.flagRegister.signFlag = (cpu.B_Register.data < 0);
                cpu.flagRegister.partyFlag = is_bits_even(cpu.B_Register.data);
                cpu.flagRegister.auxliaryCarry = is_auxiliary_carry_set(cpu.B_Register.data);

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
                unsigned short HL = (unsigned short) cpu.H_Register.data << 8 | cpu.L_Register.data;
                unsigned short BC = (unsigned short) cpu.B_Register.data << 8 | cpu.C_Register.data;

                short result = HL + BC;
                cpu.flagRegister.carryFlag = (result << HL);

                cpu.H_Register.data = (unsigned char) (result >> 8);
                cpu.L_Register.data = (unsigned char) result;

                break;
            }
            // LDAX B
            // After the execution of this command, 
            // the value stored in the memory location whose address is formed by the contents of registers B and C will be loaded into the accumulator register A.
            case 0x0a:
            {
                unsigned short targetAddress = ((unsigned short)cpu.B_Register.data << 8) | cpu.C_Register.data;
                cpu.A_Register.data = read_memory_ram(ramGateway, targetAddress - 1);
            
                break;
            }
            // DCX B
            // Decrement Double Register
            case 0x0b:
            {
                unsigned short BC = (cpu.B_Register.data << 8) | cpu.C_Register.data;
                BC--;

                cpu.B_Register.data = (BC >> 8) & 0xFF;
                cpu.C_Register.data = BC & 0xFF;

                break;
            }
        }

        cpu.programCounter.data++;
    }
}
