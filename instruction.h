#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Instruction{
public:
    std::string label;
    std::string operand;
    std::string opcode;

    // Constructor
    Instruction(std::string lbl = "", std::string opc = "", std::string opr = "");

    // Function to write to a listing file
    void writeToListing(std::ofstream &listingFile) const;
};

#endif