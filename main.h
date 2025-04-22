//Alejandro Pacheco, Taiki Shank
//cssc0803, cssc5097
//CS530, Spring 2025
//Assignment #2, Two-Pass Assembler
//main.h

#ifndef MAIN_H
#define MAIN_H

// Import statements
#include <iostream>
#include <ostream>
#include <iomanip>
#include <ctype.h>
#include <bitset>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

// Constant variable definitions
#define ERROR_RETURN_CODE (-1) // Exit statement for errors
#define RETURN_CODE 0          // Exit statement constant

std::string getFileName(std::string); // Function to remove .sic from file path

#endif // MAIN_H