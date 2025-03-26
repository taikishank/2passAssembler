#include "instruction.h"


// Constructor
Instruction::Instruction(std::string lbl, std::string instr, std::string opd, int addr)
    : label(lbl), instruction(instr), operand(opd), address(addr){}

int Instruction::reserve_address_bytes(){
    if (instruction[0] == '+')
        return 4;
    else if (format2_instructions.find(instruction) != format2_instructions.end())
        return 2;
    else if (instruction == "RESB")
        return stoi(operand);
    else if (instruction == "RESW")
        return stoi(operand) * 3;
    else if (instruction == "START")
        return 0;
    else if (instruction == "BYTE")
        if (operand[0] == 'C')
            return operand.size() - 3;
        else if (operand[0] == 'X')
            return (operand.size() - 3) / 2;
        else{
            std::cout << "You're fucking stupid";
            exit(0);
        }
    return 3;
}