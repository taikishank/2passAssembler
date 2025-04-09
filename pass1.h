#ifndef PASS1_H
#define PASS1_H

#include <string>
#include <vector>
#include <fstream>

class Instruction; // Forward declaration

// Parses the given file and populates a vector of Instruction pointers.
// Also writes each instruction to the provided listing file.

void pass1(const std::string &filename, std::ofstream &listingFile);
int writeToListing(Instruction *instruction, int current_address, std::ofstream &listingFile);
#endif // PASS1_H