#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "instruction.h"
#include "pass1.h"
#include <iomanip>
#include <iostream>
#include "labelMap.h"

void pass2(std::ofstream& listingFile, std::vector <Instruction *> instructionList)
{
    int i = 1;
}

std::string formatOneOpcode(Instruction *instr){
    return opcodeTable[instr->instruction].first;
}

std::string formatTwoOpcode(Instruction *instr){
    //TODO: add edge cases
    std::string base = opcodeTable[instr->instruction].first;
    std::vector<std::string> res;
    std::stringstream ss(instr->operand);
    std::string token;
    while (std::getline(ss, token, ',')){
        res.push_back(token);
    }
    int r1 = registerTable[res[0]];
    int r2 = registerTable[res[1]];
    std::stringstream output;
    output << base << r1 << r2;
    return output.str();
}

int formatThreeOpcode(Instruction *instr){
    int n, i, x, b, p; // e always 0 in F3
    int base = std::stoi(opcodeTable[instr->instruction].first);    // convert string into int
    if (instr->instruction[0] == '@'){
        n = 0;
        i = 1;
    }    
    else if (instr->operand[0] == '#'){
        n = 1;
        i = 0;
    } 
    if (instr->operand.find(',') != std::string::npos)   // look for ',' indicating indeXed
        x = 0;
    else   
        x = 1;
    // probably PC relative most of time time???
    b = 0;
    p = 1;

    //TODO use flags with opcode

}
int formatFourOpcode(Instruction *instr){

}

// for every instruction in instruction list 

// generate an opcode for that instruction
// traverse through the current line, reach the end add space, insert opcode 
// Each format has its own algorithm
    // (Function)