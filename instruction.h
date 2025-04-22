//Alejandro Pacheco, Taiki Shank
//cssc0803, cssc5097
//CS530, Spring 2025
//Assignment #2, Two-Pass Assembler
//instruction.h

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

class Instruction
{
public:
// Instruction parameters
    std::string label;
    std::string operand;
    std::string instruction;
    int address;
    
    std::string instructionListingInfo;

    // Constructor
    Instruction(std::string lbl = "", std::string opc = "", std::string opr = "", int address = -1);

    // Function to reserve bytes based on format, assembler directives
    int reserve_address_bytes();
};

#endif