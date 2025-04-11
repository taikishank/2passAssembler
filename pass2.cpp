#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "instruction.h"
#include "pass1.h"
#include <iomanip>

void pass2(std::ofstream& listingFile, std::vector <Instruction *> instructionList)
{
    int i = 1;
}

int formatOneOpcode(Instruction *instr){

}
int formatTwoOpcode(Instruction *instr){

}
int formatThreeOpcode(Instruction *instr){

}
int formatFourOpcode(Instruction *instr){

}

// for every instruction in instruction list 

// generate an opcode for that instruction
// traverse through the current line, reach the end add space, insert opcode 
// Each format has its own algorithm
    // (Function)