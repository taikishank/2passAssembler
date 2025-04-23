// Alejandro Pacheco, Taiki Shank
// cssc0803, cssc0830
// CS530, Spring 2025
// Assignment #2, Two-Pass Assembler
// labelMap.h

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <map>
#include <set>
#include <string>
#include <utility> // Include for std::pair

extern std::map<std::string, int> registerTable; // Used for register value pairs
extern std::map<std::string, std::pair<std::string, int>> opcodeTable; // Declaration of the opcode pairings
extern std::set<std::string> assembler_directives; // Set of all valid assembler directives
extern std::map<std::string, int> symbolTable; // Used for pass 2 addressing for labels and literals
#endif