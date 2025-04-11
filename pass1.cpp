#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "instruction.h"
#include "pass1.h"
#include <iomanip>
#include "labelMap.h"

std::vector<Instruction *> pass1(const std::string &filename, std::ofstream &listingFile)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cout << "Unable to open file " << filename << ". Shutting down." << std::endl;
        exit(-1);
    }

    int address = 0x0000; // we have to update this with the first instruction (START)

    std::string line;
    std::vector<Instruction *> instruction_list;
    std::vector<Instruction *> label_list;
    std::vector<Instruction *> literal_list;
    while (std::getline(file, line))
    {
        if(line[0] == '.')
        {
            listingFile << line << std::endl;
            continue;
        }
        std::istringstream iss(line);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word)
        {
            words.push_back(word);
        }

        
        
        Instruction *instr;

        if (words.size() == 1){
            std::cout << 1 << "    " << line << std::endl;
            instr = new Instruction("", words[0], "", address);
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
        }
        else if (words.size() == 2)
        {
            std::cout << 2 << "    " << line << std::endl;
            
            
            instr = new Instruction("", words[0], words[1], address);
            
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
        }
        else if (words.size() == 3)
        { // Update symbol table here
            std::cout << 3 << "    " << line << std::endl;
            instr = new Instruction(words[0], words[1], words[2], address);
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
            symbolTable[instr->label] = instr->address;
            label_list.push_back(instr);
        }
        else
        {
            std::cout << "ERROR: Invalid input format. Line must contain either 2 or 3 words" << std::endl;
            exit(-1);
        }
    }
    
    Instruction* last = instruction_list.back();

    // Generate symbol table
    int size = last->address;
    std::ofstream symbol_table_file = initialize_symbol_table(filename);

    for( Instruction* instr: label_list){
        std::cout << "Generating symbol table for: " << instr->label << instr->instruction << std::endl;
        generateSymbolTable(instr, size, symbol_table_file);
    }

    initializeLiteralTable(symbol_table_file);
    for(Instruction* instr: literal_list){
        std::cout << "Generating literal table for: " << instr->label << instr->instruction << std::endl;
        generateLiteralTable(instr);
    }

    //return listingFile;
    return instruction_list;
}


std::ofstream initialize_symbol_table(const std::string &filename)
{
    std::stringstream listing_file_name;
    // file_name = getFileName(curr_file); // TODO: Implement this function to get the file name from the path
    listing_file_name << filename << "TESTFILE.st"; // REMOVE TESTFILE LATER
    std::string listing_file = listing_file_name.str();
    std::ofstream listingFile(listing_file);
    if (!listingFile)
    {
        std::cerr << "Unable to open symbol table file" << std::endl;
        exit(-1);
    }
    listingFile << "CSect   Symbol  Value   LENGTH  Flags:" << std::endl;
    listingFile << "--------------------------------------" << std::endl;

    return listingFile;
}

void generateSymbolTable(Instruction *instr, int size, std::ofstream &listingFile)
{
    if(instr->instruction == "START"){
        int label_padding = std::max(0, 6 - (int)instr->label.length());
        std::string label_whitespace(label_padding, ' ');

        listingFile << instr->label << label_whitespace << "  " << "      " << "  " 
        << std::hex << std::setw(6) << std::setfill('0') << std::uppercase << instr->address 
        << "  " << size << "  " << std::endl;
    }
    else{
        int label_padding = std::max(0, 6 - (int)instr->label.length());
        std::string label_whitespace(label_padding, ' ');

        listingFile << "      " << "  " <<
        instr->label << label_whitespace << "  " 
                    << std::hex << std::setw(6) << std::setfill('0') << std::uppercase << instr->address
                    << "  " 
                    << "      " << "  "
                    << "R" << std::endl;
    }
}
void initializeLiteralTable(std::ofstream &symbolFile)
{
    symbolFile << "" << std::endl;
    symbolFile << "Literal Table"  << std::endl;
    symbolFile << "Name  Operand   Address  Length:" << std::endl;
    symbolFile << "--------------------------------" << std::endl;
}

void generateLiteralTable(Instruction *instruction){

}

int writeToListing(Instruction *instruction, int current_address, std::ofstream &listingFile)
{
    if (!listingFile)
    {
        std::cout << "Listing file is not open! Open that up!" << std::endl;
        return -1;
    }
    if(instruction->instruction == "END")
    {
        listingFile <<  "                  " << instruction->instruction 
        << "       " << instruction->operand << std::endl;
        return current_address;
    }

    if (!instruction->label.empty() || !instruction->instruction.empty())
    {
        int label_padding = std::max(0, 6 - (int)instruction->label.length());
        std::string label_whitespace(label_padding, ' ');
        int instruction_padding = std::max(0, 6 - (int)instruction->instruction.length());
        std::string instruction_whitespace(instruction_padding, ' ');

        listingFile << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << instruction->address << "    " <<
         instruction->label << label_whitespace << "    " <<
         instruction->instruction << instruction_whitespace << "    " <<
         instruction->operand << std::endl;
        int increment = instruction->reserve_address_bytes();
        
        current_address += increment;
        return current_address;
    }
    return current_address;
}