#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

class Instruction{
public:
    std::string label;
    std::string operand;
    std::string instruction;
    std::set<std::string> format2_instructions;

    // Constructor
    Instruction(std::string lbl = "", std::string opc = "", std::string opr = "");

    // Function to write to a listing file
    void writeToListing(std::ofstream &listingFile) const;

    int format_check(std::string instruction, std::string operand);
};

#endif