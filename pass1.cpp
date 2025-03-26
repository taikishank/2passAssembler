#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "instruction.h"

void pass1(const std::string &filename, std::ofstream &listingFile){
    std::ifstream file(filename);
    if (!file){
        std::cout << "Unable to open file " << filename << ". Shutting down." << std::endl;
        return;
    }

    int address = 0x0000; // we have to update this with the first instruction (START)

    std::string line;
    while (std::getline(file, line)){
        std::istringstream iss(line);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word){
            words.push_back(word);
        }
        
        std::vector<Instruction*> instruction_list;
        Instruction* instr;

        if (words.size() == 2){
            instr = new Instruction("", words[0], words[1], address);
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
        }
        else if (words.size() == 3){ // Update symbol table here 
            instr = new Instruction(words[0], words[1], words[2], address);
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
        }
        else{
            std::cout << "ERROR: Invalid input format. Line must contain either 2 or 3 words" << std::endl;
            return;
        }
    }
}

int writeToListing(Instruction* instruction, int current_address, std::ofstream &listingFile) {
    if (!listingFile){
        std::cout << "Listing file is not open! Open that shit up dumbass!" << std::endl;
        return;
    }

    if (!instruction->label.empty()){
        int increment = instruction->reserve_address_bytes();
        listingFile << instruction->address + increment << "\t" << instruction->label << "\t" << instruction->instruction << "\t" << instruction->operand << std::endl;
        current_address += increment;
        return current_address;
    }
    else{
        listingFile << instruction->address << "\t\t" << instruction->instruction << "\t" << instruction->operand << std::endl; // formatting may be off, won't know until testing
    }
    return;
}