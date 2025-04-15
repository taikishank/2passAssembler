#ifndef PASS2_H
#define PASS2_H

#include <string>
#include <vector>
#include <fstream>

#include "instruction.h"
#include "labelMap.h"

void pass2(std::ofstream &listingFile, std::vector<Instruction *> instructionList);
std::string formatOneOpcode(Instruction *instr);
int formatTwoOpcode(Instruction* instr);
int formatThreeOpcode(Instruction *instr);
int formatFourOpcode(Instruction *instr);

#endif // PASS2_H