// Alejandro Pacheco, Taiki Shank
// cssc0803, cssc5097
// CS530, Spring 2025
// Assignment #2, Two-Pass Assembler
// main.cpp

#include "main.h"
#include "pass1.h"
#include "pass2.h"
#include "instruction.h"
#include "labelMap.h"

/*
This file contains the logic that connects pass 1 and pass 2, as well as handles the command line processing
and any error handling. 
*/
int main(int argc, char **argv)
{
    // If the total number of command line arguments are less than 2, we know there wasn't a file path inputted
    // Therefore, this shall return an error message
    if (argc < 2)
    {
        std::cerr << "No file path is provided. Please include a valid .sic file." << std::endl;
        return ERROR_RETURN_CODE;
    }

    std::vector<std::string> file_paths; // There can be more than one .sic file we need to process, so we will add it to a list 
    for (int i = 1; i < argc; i++)
    {
        file_paths.push_back(argv[i]);
    }

    // For every file in the vector
    for (std::string curr_file : file_paths)
    {
        
        FILE *fp = fopen(curr_file.c_str(), "r"); // Open the file
        if (fp == nullptr) // If the current file does not exist, we will terminate the program and return an error message 
        {
            std::cerr << "Unable to open " << curr_file << std::endl;
            return ERROR_RETURN_CODE;
        }

        // At this point, we know we have a valid file that exists in our folder, and we will now create a sample listing file to use and pass to pass one.
        std::stringstream listing_file_name; // A string stream to serve as the listing file (.l)
        std::string file_name = getFileName(curr_file); // Removes the .sic from the ending of the file path
        listing_file_name << file_name << ".l";         // Adds .l to the file path
        std::string listing_file = listing_file_name.str(); // Converts the string stream into a string
        std::ofstream listingFile(listing_file); 
        if (!listingFile) // If the listing file is invalid, we will return an error message
        {
            std::cerr << "Unable to open listing file" << std::endl;
            return ERROR_RETURN_CODE;
        }

        std::vector<Instruction *> instrList = pass1(curr_file, listingFile); // PASS ONE

        pass2(listingFile, instrList); // PASS TWO

        symbolTable.clear(); // Clear the current symbol table that we produced in pass 1/2 for the next iteration

        listingFile.close(); // Close the current listing file to be able to open the next one
    }

    return RETURN_CODE; // Return 0, indicating file finished printing
}

// Function to remove the .sic from the file path
std::string getFileName(std::string f)
{
    std::string file = f.substr(0, f.length() - 4);
    return file;
}