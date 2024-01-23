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

    cpu.flagRegister.data = 0;

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
                cpu.C_Register.data++;
                break;
            // INR B
            case 0x04:
                cpu.B_Register.data++;
        }

        cpu.programCounter.data++;
    }
}
