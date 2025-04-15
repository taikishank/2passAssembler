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

std::string formatThreeOpcode(Instruction *instr){
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
    int disp = instr->address - 3;  // assuming PC, TA - PC = TA - 3
    if (disp >= -2048 && disp <= 2047) {
        b = 0;
        p = 1;
    } else {
        b = 1;
        p = 0;
    }
    
    std::stringstream ss;
    int opcode = (base & 0xFC) | (n << 1) | i;  // 0xFC = 11111100
    ss << std::setw(2) << opcode;
    int flags = (x << 3) | (b << 2) | (p << 1);
    int byte2 = (flags << 4) | ((disp >> 8) & 0x0F);
    ss << std::setw(2) << byte2;

    return ss.str();
}
int formatFourOpcode(Instruction *instr){

}

// for every instruction in instruction list 

// generate an opcode for that instruction
// traverse through the current line, reach the end add space, insert opcode 
// Each format has its own algorithm
    // (Function)