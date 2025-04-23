// Alejandro Pacheco, Taiki Shank
// cssc0803, cssc0830
// CS530, Spring 2025
// Assignment #2, Two-Pass Assembler
// instruction.cpp

#include "instruction.h"
#include "labelMap.h"

/*
This file contains a constructor for the instruction, which represents an individual line in the 
.sic file. It also contains a function that will calculate the total number of bits (space) that the 
instruction takes.
*/


// Constructor for instruction
Instruction::Instruction(std::string lbl, std::string instr, std::string opd, int addr)
    : label(lbl), instruction(instr), operand(opd), address(addr) {}


// Function to reserve bytes based on format, assembler directives    
// Will return the size of the instruction to update the address in pass 1
int Instruction::reserve_address_bytes()
{
    // If the current instruction is in the opcode table, determine its size
    if (opcodeTable.find(instruction) != opcodeTable.end())
    {
        // This is the pair value that represents (Opcode, format)
        // We are looking for the format, therefore it will be the second value we access
        // Format 4 returns 4, format 3 returns 3, etc.
        std::pair<std::string, int> opcode = opcodeTable[instruction];
        if (opcode.second == 4)
            return 4;
        else if (opcode.second == 2)
            return 2;
        else if(opcode.second == 1)
            return 1;
        return 3;
    }

    // Otherwise, the current instruction COULD be an assembler directive
    else{
        if (assembler_directives.find(instruction) != assembler_directives.end()){
            if (instruction == "RESB") { // RESB will reserve the inputted number
                return stoi(operand);
            }
            else if (instruction == "RESW") { // RESW will reserve the inputted number * 3
                return stoi(operand) * 3;
            }
            else if (instruction == "START") // START will indicate the starting address in HEX
            {
                return std::stoi(operand, nullptr, 16);
            }
            else if (instruction == "END") // The last instruction does not have a "size"
            {
                return 0;
            }
            else if(instruction[0] == '*'){ // Used for literal instructions to store the total size
                if (operand[1] == 'C')
                {
                    return operand.size() - 4; // If its a C instruction, its just the size
                }
                else if (operand[1] == 'X')
                {
                    return (operand.size() - 4) / 2; // Otherwise, there is some slight modification needed
                }
            }
            else if (instruction == "BYTE") { // Like *, this will reserve a certain amount of space based on the operand size
                if (operand[0] == 'C'){
                    return operand.size() - 3;
                } 
                else if (operand[0] == 'X') { // Same condition for X
                    return (operand.size() - 3) / 2;
                }
            }
            else{
                return 0;
            }
            return 0;
        }
        // If it isn't in the assembler directive sets, then we have an invalid instruction and will terminate
        else{
            std::cout << "ERROR: Instruction" << instruction << "not found in symbol table" << std::endl;
            exit(0);
            return -1;
        }
    }
}