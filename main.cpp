// CS 530, Spring 2025
// Assignment #1, File Reader
// main.cpp

#include "main.h"
#include "pass1.h"
#include "pass2.h"
#include "instruction.h"
#include "labelMap.h"


int main(int argc, char **argv)
{
    std::cout << std::endl;

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
        return ERROR_RETURN_CODE;
    }

    std::vector<std::string> file_paths;
    for (int i = 1; i < argc; i++)
    {
        file_paths.push_back(argv[i]);
    }

    std::vector<std::vector<Instruction *>> instructionLists;
    
    // PASS ONE
    for (std::string curr_file : file_paths)
    {
        std::stringstream listing_file_name;
        // file_name = getFileName(curr_file); // TODO: Implement this function to get the file name from the path
        listing_file_name << curr_file << "TESTFILE.l"; // REMOVE TESTFILE LATER 
        std::string listing_file = listing_file_name.str();
        std::ofstream listingFile(listing_file);
        if (!listingFile)
        {
            std::cerr << "Unable to open listing file" << std::endl;
            return ERROR_RETURN_CODE;
        }
        FILE *fp = fopen(curr_file.c_str(), "r"); // Open the file
        if (fp == nullptr)
        {
            std::cerr << "Unable to open " << curr_file << std::endl;
            return ERROR_RETURN_CODE;
        }
        std::cout<<"Reading file: " << curr_file << std::endl;
        std::vector<Instruction *> instrList = pass1(file_paths[0], listingFile); // PASS ONE

        pass2(listingFile, instrList); // PASS TWO

        symbolTable.clear();

        listingFile.close();
    }

    return RETURN_CODE; // Return 0 indicating file finished printing
}


// IMPLEMENT THIS FUNCTION TO GET THE FILE NAME FROM THE PATH
std::string getFileName(std::string){
    return "";

}