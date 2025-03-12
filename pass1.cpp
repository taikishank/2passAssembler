#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Instruction.h"

void pass1(const std::string &filename, std::vector<Instruction*> &instructions){
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

        if (words.size() == 2){
            instructions.push_back(new Instruction("", words[0], words[1]));
        }
        else if (words.size() == 3){
            instructions.push_back(new Instruction(words[0], words[1], words[2]));
        }
        else{
            std::cout << "ERROR: Invalid input format. Line must contain either 2 or 3 words" << std::endl;
            return;
        }
    }
}

void parse2Lines(std::vector<std::string> instruction){
    // check OPTABLE (datatype map) and reference instruction for validitiy



}

void parse3Lines(){
    // pass line into object
    // create object funciton for adding to listing file
}