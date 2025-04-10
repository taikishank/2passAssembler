#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "instruction.h"
#include "pass1.h"
#include <iomanip>

void pass1(const std::string &filename, std::ofstream &listingFile)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cout << "Unable to open file " << filename << ". Shutting down." << std::endl;
        return;
    }

    int address = 0x0000; // we have to update this with the first instruction (START)

    std::string line;
    while (std::getline(file, line))
    {
        if(line[0] == '.')
        {
            listingFile << line << std::endl;
            continue;
        }
        std::istringstream iss(line);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word)
        {
            words.push_back(word);
        }

        std::vector<Instruction *> instruction_list;
        Instruction *instr;

        // NEED TO FIX FORMATTING ISSUE FOR ADDRESSES
        // use psample.lst as a reference

        if (words.size() == 1){
            std::cout << address << "    " << line << std::endl;
            instr = new Instruction("", words[0], "", address);
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
        }
        else if (words.size() == 2)
        {
            std::cout << address << "    " << line << std::endl;
            instr = new Instruction("", words[0], words[1], address);
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
        }
        else if (words.size() == 3)
        { // Update symbol table here
            std::cout << address << "    " << line << std::endl;
            instr = new Instruction(words[0], words[1], words[2], address);
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
        }
        else
        {
            std::cout << "ERROR: Invalid input format. Line must contain either 2 or 3 words" << std::endl;
            return;
        }
    }
}

int writeToListing(Instruction *instruction, int current_address, std::ofstream &listingFile)
{
    if (!listingFile)
    {
        std::cout << "Listing file is not open! Open that up!" << std::endl;
        return -1;
    }
    if(instruction->instruction == "END")
    {
        listingFile <<  "\t" "\t" << instruction->label << "\t" << instruction->instruction << "\t" << instruction->operand << std::endl;
        return current_address;
    }

    if (!instruction->label.empty() || !instruction->instruction.empty())
    {
        listingFile << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << instruction->address << "    " << instruction->label << "\t" << instruction->instruction << "\t" << instruction->operand << std::endl;
        int increment = instruction->reserve_address_bytes();
        
        current_address += increment;
        return current_address;
    }
    else
    {
        std::cout << "Address not incremented" << std::endl;
        listingFile << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << instruction->address << "\t\t" << instruction->instruction << "\t" << instruction->operand << std::endl; // formatting may be off, won't know until testing
    }
    return current_address;
}