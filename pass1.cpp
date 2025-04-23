// Alejandro Pacheco, Taiki Shank
// cssc0803, cssc0830
// CS530, Spring 2025
// Assignment #2, Two-Pass Assembler
// pass1.cpp

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

/*
This file contains all the logic essential to pass 1 of the two pass assembler. This file generates 
- A symbol table
- Information for the listing file
- Confirms instructions and labels are valid
- Generates addresses for each instruction and assembler directive (if applicable)
*/

// This function will simulate pass one, and returns a list of instructions that will be used for pass 2
std::vector<Instruction *> pass1(const std::string &filename, std::ofstream &listingFile)
{
    // We want to ensure here again that the file we sent is valid, so we will check again.
    std::ifstream file(filename);
    if (!file)
    {
        std::cout << "Unable to open file " << filename << ". Shutting down." << std::endl;
        exit(-1);
    }

    std::stringstream literalTable; // Initializing the literal table early here alongside other declarations of important values
    literalTable << "\nLiteral Table\nName  Operand   Address  Length:\n--------------------------------\n";

    int address = 0; // Current address for the current instruction

    std::string line; // Serves as the current line in the current file we are reading through
    std::vector<Instruction *> instruction_list; // Contains the list of instructions in the current file that will be sent to pass 2
    std::vector<Instruction *> label_list; // List of all the labels we will use for the symbol table (Instruction has 3 parameters)
    std::vector<Instruction *> literal_list; // List of all literals that are collected by the first pass

    while (std::getline(file, line)) // Traverse through every line in the file
    {
        // To prevent redundancy, each case of the instruction will have a string stream called "listing" which will contain information about the current
        // instruction (address, label, instruction, operand) serving as an intermediate file and getting stored in the current instruction

        Instruction *instr; // Sets an empty instruction for each of the cases
        if(line[0] == '.') // If the line is a comment, we well create an empty instruction so we can ensure we write it in pass 2
        {
            instr = new Instruction("", ".", "", address); 

            std::stringstream listing; // Creates a new stream of the current comment

            listing << line; // Adds the the comment
            instr->instructionListingInfo = listing.str();
            instruction_list.push_back(instr); // Adds it to the list

            continue; // We can skip the rest of the logic for the comments
        }
        // Since this isn't a comment, we want to continue through all the values, storing other information. 
        std::istringstream iss(line);
        std::vector<std::string> words; // Will contain a vector of the line's parameters (label, instruction, operand) for those that exist
        std::string word;

        while (iss >> word) // While we can continue to add a non-whitespace value, push it back to the words vector
        {
            words.push_back(word);
        }

        // For each of these, we will update the address with the new value of the instruction + current address
        if (words.size() == 1){ // If the size is one, simply create a new instruction, call the write to listing file, and add the instruction to the list
            instr = new Instruction("", words[0], "", address);
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
        }
        else if (words.size() == 2) // If the size is two, create a new instruction with the operand, call the write to listing file, and add the instruction to the list
        {
            instr = new Instruction("", words[0], words[1], address);
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
        }
        else if (words.size() == 3) // If the size is three, create a new instruction with the operand and label, call the write to listing file, and add the instruction to the list
        { 
            instr = new Instruction(words[0], words[1], words[2], address);
            address = writeToListing(instr, address, listingFile);
            instruction_list.push_back(instr);
            symbolTable[instr->label] = instr->address; // Due to us having a label, we must add that label to the symbol table and the instruction's corresponding address too. 
            label_list.push_back(instr);
        }
        else // Otherwise, we have an invalid input and cannot continue to process the faulty file format. 
        {
            std::cout << "ERROR: Invalid input format. Line must contain either 1, 2 or 3 words" << std::endl;
            exit(-1);
        }
        
        if (instr->instruction[0] == '*' && instr->operand[0] == '='){ // If we have this type of assembler directive, we will write something else
            
            if (instr->operand[1] == 'X'){
                std::string op = instr->operand.substr(3, instr->operand.size() - 4); // Extracts the information from the operand =X'04' -> 04
                int length = op.size(); // Gets the length of this value for the padding

                // Generates padding between the operand and next value for the literal table
                int name_padding = std::max(0, 3 - length); 
                std::string name_whitespace(name_padding, ' ');

                // Generates padding between the operand and the address for the literal table
                int operand_padding = std::max(0, 6 - length);
                std::string label_whitespace(operand_padding, ' ');

                // Combine all the values together in its corresponding order and add it to the literal table entry. 
                // std::hex turns the address into hex
                // Setw and setfill provides padding of zeroes
                literalTable << op << name_whitespace << "   "
                             << op << label_whitespace << "    " << std::hex << std::setw(4) << std::setfill('0') <<
                             instr->address << "     " << length << std::endl;

                
            }
            else{ // 'C'
                std::string op = instr->operand.substr(3, (instr->operand.size() - 4)); // Extracts the information from the operand (=C'EOF') -> EOF
                int length = op.size(); // Gets the length of this value for the padding

                // Generates padding between the operand and next value for the literal table
                int name_padding = std::max(0, 3 - length);
                std::string name_whitespace(name_padding, ' ');

                // Generates padding between the operand and the address for the literal table
                int operand_padding = std::max(0, 6 - (length * 2));
                std::string operand_whitespace(operand_padding, ' ');

                // Converts every character to its corresponding ASCII value
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
    int size = last->address; // Gets the last address in the input file, which corresponds to the length of our program

    std::ofstream symbol_table_file = initialize_symbol_table(filename); // Creates a new symbol table for the current file

    for( Instruction* instr: label_list){ // For every label in the current file, we will create an entry in the symbol table
        generateSymbolTable(instr, size, symbol_table_file);
    }

    // At the end, we can just merge the symbol table with the literal table we have been generating throughout the previous code
    symbol_table_file << literalTable.str();

    return instruction_list;
}

// This function simply initializes the framework for the symbol table. Just deals with formatting and spaces
std::ofstream initialize_symbol_table(const std::string &filename)
{
    std::stringstream listing_file_name;
    std::string stripped_file_name = getFileName(filename); // Gets the stripped file name without any .() values
    listing_file_name << stripped_file_name << ".st";       // Creates a new .st file for the current file
    std::string listing_file = listing_file_name.str();
    std::ofstream listingFile(listing_file);
    if (!listingFile)
    {
        std::cerr << "Unable to open symbol table file" << std::endl;
        exit(-1);
    }
    listingFile << "CSect   Symbol  Value   LENGTH  Flags:" << std::endl; // Adds header
    listingFile << "--------------------------------------" << std::endl; // Adds spacing

    return listingFile; // Return the stream to add new values as we come across them
}

// This function generates a singular entry in the symbol table for each label
void generateSymbolTable(Instruction *instr, int size, std::ofstream &listingFile)
{
    // If its a START assembler directive, we must handle and format it slightly differently
    if(instr->instruction == "START"){
        // It will store the label, certain padding based on the program name size, and its length 
        int label_padding = std::max(0, 6 - (int)instr->label.length());
        std::string label_whitespace(label_padding, ' ');


            listingFile << instr->label << label_whitespace << "  "
                << "      "
                << "  "
                << std::setw(6) << std::setfill('0') << std::hex << std::uppercase << instr->address
                << "  " << size << "  " << std::endl;

    }
    else{
        // Otherwise it will follow a stuctured format of the label, several space, and the address with its corresponding type.
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

// The write to listing file will take in a current instruction, the current address, and the listing file to write in
int writeToListing(Instruction *instruction, int current_address, std::ofstream &listingFile)
{
    // If the listing file is invalid:
    if (!listingFile)
    {
        std::cout << "Listing file is not open! Open the file!" << std::endl;
        return -1;
    }
    if (instruction->instruction == "START") // If we are at the start, we have a different process to write into the listing file
    {
        std::stringstream listing;

        int startingAddress = instruction->reserve_address_bytes(); // We will find the actual starting address, which will be the third parameter in the START line

        // Generates padding based on the size of the label
        int label_padding = std::max(0, 6 - (int)instruction->label.length());
        std::string label_whitespace(label_padding, ' ');

        // Generates padding based on the size of the instruction
        int instruction_padding = std::max(0, 6 - (int)instruction->instruction.length());
        std::string instruction_whitespace(instruction_padding, ' ');

        // Combines the information together based on the white space and instruction parameters
        listing << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << startingAddress << "    " << instruction->label << label_whitespace << "   ";

        // Adds the reset of the information based on the general format
        listing << instruction->instruction << instruction_whitespace << "   " << instruction->operand;

        // Sets the parameter in the instruction to this string which will be fully added to the listing file in pass 2
        instruction->instructionListingInfo = listing.str();

        // Return the starting address that we calculated in reserve_address_bytes
        return startingAddress;
    }
    if(instruction->instruction == "END")
    {
        // If we're at the end of the file, we just need to include END (label) and add that parameter to instruction
        std::stringstream listing;

        listing << "                 " + instruction->instruction + "      " + instruction->operand;
        instruction->instructionListingInfo = listing.str();

        return current_address;
    }
    
    if (instruction->instruction[0] == '*' && instruction->operand[0] == '=') // If we have this assembler directive, we follow a similar process but must shift the instruction to the left
    {
        // Generates padding based on the size of the label
        int label_padding = std::max(0, 6 - (int)instruction->label.length());
        std::string label_whitespace(label_padding, ' ');

        // Generates padding based on the size of the instruction
        int instruction_padding = std::max(0, 6 - (int)instruction->instruction.length());
        std::string instruction_whitespace(instruction_padding, ' ');
        
        // Include all the information into the listing stream
        std::stringstream listing;
        listing << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << instruction->address << "    " <<
         instruction->instruction << label_whitespace << " " << instruction->operand;
        instruction->instructionListingInfo = listing.str();

        std::string op = instruction->operand.substr(3, instruction->operand.size() - 4); // This will calculate the current value of the literal
        symbolTable[op] = current_address; // This add the current literal to the symbol table for pass 2    

        return current_address + (instruction->operand.size() - 4); // Returns the size of the operand based by removing ', ', =, and (C or X)
    }

    if (!instruction->label.empty() || !instruction->instruction.empty()) // This ensures that the instruction has a label or instruction to execute upon
    {
        // Like the other formats, we need to ensure we have adequate and consistent padding between all the values

        // Generates padding based on the size of the label
        int label_padding = std::max(0, 6 - (int)instruction->label.length());
        std::string label_whitespace(label_padding, ' ');

        // Generates padding based on the size of the instruction
        int instruction_padding = std::max(0, 6 - (int)instruction->instruction.length());
        std::string instruction_whitespace(instruction_padding, ' ');

        std::stringstream listing;

        // Add the information to the listing stream
        listing << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << instruction->address << "    " <<
         instruction->label << label_whitespace << "  ";

         // Unlike the other formats, we need to account for '+', '@', '#', and '=' and adjust the padding accordingly
        if (instruction->instruction[0] != '+'){ // If there is not a '+' in the instruction, we can insert an extra space 
            listing << " ";
        }

        listing << instruction->instruction << instruction_whitespace << "  ";

         if (instruction->instruction[0] == '+') // If there is a + in the instruciton, we insert an extra space AFTER we include the instruction with the padding
         {
             listing << " ";
         }

         if (instruction->operand[0] != '#' && instruction->operand[0] != '@' && instruction->operand[0] != '=') // If there is not any of these values, we insert an extra space
         {
             listing << " ";
         }

         listing << instruction->operand; // Add the operand to the listing file

        instruction->instructionListingInfo = listing.str(); // Set the instruciton parameter to this value

        int increment = instruction->reserve_address_bytes(); // Increment the current value based on its corresponding format type
    
        
        current_address += increment;
        return current_address; // Return the new address
    }
    return current_address; // No update was needed
}