//Alejandro Pacheco, Taiki Shank
//cssc0803, cssc5097
//CS530, Spring 2025
//Assignment #2, Two-Pass Assembler
//pass1.h

#ifndef PASS1_H
#define PASS1_H

#include <string>
#include <vector>
#include <fstream>

class Instruction; // Forward declaration

// Parses the given file and populates a vector of Instruction pointers.
// Also writes each instruction to the provided listing file.
std::vector<Instruction *> pass1(const std::string &filename, std::ofstream &listingFile);
std::ofstream initialize_symbol_table(const std::string &filename); // Creates framework of symbol table
void generateSymbolTable(Instruction *instruction, int size, std::ofstream &listingFile); // Creates symbol table
int writeToListing(Instruction *instruction, int current_address, std::ofstream &listingFile); // Writes information into listing file instruction parameter
#endif // PASS1_H