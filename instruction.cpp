#include "instruction.h"
#include "labelMap.h"

// Constructor
Instruction::Instruction(std::string lbl, std::string instr, std::string opd, int addr)
    : label(lbl), instruction(instr), operand(opd), address(addr) {}

int Instruction::reserve_address_bytes()
{
    if (symbolTable.find(instruction) != symbolTable.end()){
        std::pair<std::string, int> opcode = symbolTable[instruction];
        if (opcode.second == 4)
            return 4;
        else if (opcode.second == 2)
            return 2;
        else if(opcode.second == 1)
            return 1;
        return 3;
    }
    else{
        if (assembler_directives.find(instruction) != assembler_directives.end()){
            if (instruction == "RESB") {
                return stoi(operand);
            }
            else if (instruction == "RESW") {
                return stoi(operand) * 3;
            }
            else if (instruction == "START")
            {
                return 0; // Change to the actual start address
            }
            else if (instruction == "END")
            {
                return 0;
            }
            else if (instruction == "BYTE") {
                if (operand[0] == 'C'){
                    return operand.size() - 3;
                }
                else if (operand[0] == 'X') {
                    return (operand.size() - 3) / 2;
                }
            }
            else{
                return 0;
            }
        }
        else{
            std::cout << "ERROR: Instruction not found in symbol table" << std::endl;
            exit(0);
            return -1;
        }
    }
}