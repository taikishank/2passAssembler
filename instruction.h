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
    int address;
    std::set<std::string> format2_instructions;

    // Constructor
    Instruction(std::string lbl = "", std::string opc = "", std::string opr = "", int address = -1);

    // Function to write to a listing file
    void writeToListing(int address, std::ofstream &listingFile);

    int reserve_address_bytes();
};

#endif