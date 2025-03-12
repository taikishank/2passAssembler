#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "instruction.h"

void pass1(const std::string &filename, std::vector<Instruction*> &instructions, std::ofstream &listingFile){
    std::ifstream file(filename);
    if (!file){
        std::cout << "Unable to open file " << filename << ". Shutting down." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)){
        std::istringstream iss(line);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word){
            words.push_back(word);
        }

        Instruction *instr = nullptr;

        if (words.size() == 2){
            instr = new Instruction("", words[0], words[1]);
        }
        else if (words.size() == 3){
            instr = new Instruction(words[0], words[1], words[2]);
        }
        else{
            std::cout << "ERROR: Invalid input format. Line must contain either 2 or 3 words" << std::endl;
            return;
        }

        instructions.push_back(instr);
        instr->writeToListing(listingFile);
    }
}