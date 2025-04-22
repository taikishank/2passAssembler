//Alejandro Pacheco, Taiki Shank
//cssc0803, cssc5097
//CS530, Spring 2025
//Assignment #2, Two-Pass Assembler
//pass2.h

#ifndef PASS2_H
#define PASS2_H

#include <string>
#include <vector>
#include <fstream>

#include "instruction.h"
#include "labelMap.h"

void pass2(std::ofstream &listingFile, std::vector<Instruction *> instructionList); // Contains object code generation and listing file completion
std::string formatOneOpcode(Instruction *instr); // Returns object code for format one
std::string formatTwoOpcode(Instruction *instr); // Returns object code for format two
std::string formatThreeOpcode(Instruction *instr, std::string base); // Returns object code for format three
std::string formatFourOpcode(Instruction *instr); // Returns object code for format four

#endif // PASS2_H