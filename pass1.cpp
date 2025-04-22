//Alejandro Pacheco, Taiki Shank
//cssc0803, cssc5097
//CS530, Spring 2025
//Assignment #2, Two-Pass Assembler
//pass1.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "instruction.h"
#include "pass1.h"
#include <iomanip>
#include "labelMap.h"
#include "main.h"

std::vector<Instruction *> pass1(const std::string &filename, std::ofstream &listingFile)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cout << "Unable to open file " << filename << ". Shutting down." << std::endl;
        exit(-1);
    }

    std::stringstream literalTable;
    
    literalTable << "\nLiteral Table\nName  Operand   Address  Length:\n--------------------------------\n";

    int address = 0;

    std::string line;
    std::vector<Instruction *> instruction_list;
    std::vector<Instruction *> label_list;
    std::vector<Instruction *> literal_list;
    while (std::getline(file, line))
    {
        Instruction *instr;
        if(line[0] == '.')
        {
            instr = new Instruction("", ".", "", address);
            //listingFile << line << std::endl;

            std::stringstream listing;

            listing << line;
            instr->instructionListingInfo = listing.str();
            instruction_list.push_back(instr);

            continue;
        }
        std::istringstream iss(line);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word)
        {
            words.push_back(word);
        }

        if (words.size() == 1){
            instr = new Instruction("", words[0], "", address);
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
        }
        else if (words.size() == 2)
        {
            instr = new Instruction("", words[0], words[1], address);
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
        }
        else if (words.size() == 3)
        { 
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
        
        if (instr->instruction[0] == '*' && instr->operand[0] == '='){
            
            if (instr->operand[1] == 'X'){
                std::string op = instr->operand.substr(3, instr->operand.size() - 4);
                int length = op.size();

                int name_padding = std::max(0, 3 - length);
                std::string name_whitespace(name_padding, ' ');

                int operand_padding = std::max(0, 6 - length);
                std::string label_whitespace(operand_padding, ' ');

                literalTable << op << name_whitespace << "   "
                             << op << label_whitespace << "    " << std::hex << std::setw(4) << std::setfill('0') <<
                             instr->address << "     " << length << std::endl;

                
            }
            else{ // 'C'
                std::string op = instr->operand.substr(3, (instr->operand.size() - 4));

                int length = op.size();
                int name_padding = std::max(0, 3 - length);
                std::string name_whitespace(name_padding, ' ');

                int operand_padding = std::max(0, 6 - (length * 2));
                std::string operand_whitespace(operand_padding, ' ');

                literalTable << op << name_whitespace << "   ";
                for (char c : op)
                {
                    literalTable << std::hex << std::uppercase << static_cast<int>(c);
                }

                literalTable <<  operand_whitespace << "    " << std::hex << std::setw(4) << std::setfill('0') <<
                instr->address << "     " << length << std::endl;
            }
        }
    }
    
    Instruction* last = instruction_list.back();
    int size = last->address;

    std::ofstream symbol_table_file = initialize_symbol_table(filename);

    for( Instruction* instr: label_list){
        generateSymbolTable(instr, size, symbol_table_file);
    }

    symbol_table_file << literalTable.str();

    return instruction_list;
}


std::ofstream initialize_symbol_table(const std::string &filename)
{
    std::stringstream listing_file_name;
    std::string stripped_file_name = getFileName(filename); // TODO: Implement this function to get the file name from the path
    listing_file_name << stripped_file_name << ".st";       // REMOVE TESTFILE LATER
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


            listingFile << instr->label << label_whitespace << "  "
                << "      "
                << "  "
                << std::setw(6) << std::setfill('0') << std::hex << std::uppercase << instr->address
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

int writeToListing(Instruction *instruction, int current_address, std::ofstream &listingFile)
{
    if (!listingFile)
    {
        std::cout << "Listing file is not open! Open up!" << std::endl;
        return -1;
    }
    if (instruction->instruction == "START")
    {
        std::stringstream listing;

        int startingAddress = instruction->reserve_address_bytes();

        int label_padding = std::max(0, 6 - (int)instruction->label.length());
        std::string label_whitespace(label_padding, ' ');
        int instruction_padding = std::max(0, 6 - (int)instruction->instruction.length());
        std::string instruction_whitespace(instruction_padding, ' ');


        listing << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << startingAddress << "    " << instruction->label << label_whitespace << "   ";

        listing << instruction->instruction << instruction_whitespace << "   " << instruction->operand;

        instruction->instructionListingInfo = listing.str();

        return startingAddress;
    }
    if(instruction->instruction == "END")
    {
        std::stringstream listing;

        listing << "                 " + instruction->instruction + "      " + instruction->operand;
        instruction->instructionListingInfo = listing.str();

        return current_address;
    }
    
    if (instruction->instruction[0] == '*' && instruction->operand[0] == '=')
    {
        int label_padding = std::max(0, 6 - (int)instruction->label.length());
        std::string label_whitespace(label_padding, ' ');
        int instruction_padding = std::max(0, 6 - (int)instruction->instruction.length());
        std::string instruction_whitespace(instruction_padding, ' ');
        std::stringstream listing;

        listing << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << instruction->address << "    " <<
         instruction->instruction << label_whitespace << " " << instruction->operand;
        instruction->instructionListingInfo = listing.str();

        std::string op = instruction->operand.substr(3, instruction->operand.size() - 4);
        symbolTable[op] = current_address;
        

        return current_address + (instruction->operand.size() - 4); // CONFIRM THIS IS TRUE OR NOT
    }

    if (!instruction->label.empty() || !instruction->instruction.empty())
    {
        int label_padding = std::max(0, 6 - (int)instruction->label.length());
        std::string label_whitespace(label_padding, ' ');
        int instruction_padding = std::max(0, 6 - (int)instruction->instruction.length());
        std::string instruction_whitespace(instruction_padding, ' ');

        std::stringstream listing;

        

        listing << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << instruction->address << "    " <<
         instruction->label << label_whitespace << "  ";

        if (instruction->instruction[0] != '+'){
            listing << " ";
        }

        listing << instruction->instruction << instruction_whitespace << "  ";

         if (instruction->instruction[0] == '+')
         {
             listing << " ";
         }

         if (instruction->operand[0] != '#' && instruction->operand[0] != '@' && instruction->operand[0] != '=')
         {
             listing << " ";
         }

         listing << instruction->operand;

        instruction->instructionListingInfo = listing.str();

        int increment = instruction->reserve_address_bytes();
    
        
        current_address += increment;
        return current_address;
    }
    return current_address;
}