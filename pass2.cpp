//Alejandro Pacheco, Taiki Shank
//cssc0803, cssc0830
//CS530, Spring 2025
//Assignment #2, Two-Pass Assembler
//pass2.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "instruction.h"
#include "pass1.h"
#include <iomanip>
#include <iostream>
#include "labelMap.h"
#include "pass2.h"

/*
This file contains the logic for pass2 which will write values into the listing file with each instruciton's corresponding object code
*/
void pass2(std::ofstream& listingFile, std::vector <Instruction *> instructionList)
{
    std::string base = "";

    // traverse through each instruction
    for(long unsigned int i = 0; i < instructionList.size(); i++){
        Instruction *instr = instructionList[i];
        if(instr->instruction == "."){      // check for comment
            listingFile << instr->instructionListingInfo << std::endl;
            continue;
        }
        if (instr->instruction == "NOBASE"){        // check for NOBASE, resets base register
            base = "";
            listingFile << instr->instructionListingInfo << std::endl;
            continue;
        }
        if (instr->instruction == "BASE"){      // check for BASE
            base = instr->operand;
            listingFile << instr->instructionListingInfo << std::endl;
            continue;
        }
        if(instr->instruction == "END"){        // check for END
            listingFile <<  instr->instructionListingInfo;
            if (i < instructionList.size() - 1){
                listingFile << std::endl;
            }
            continue;
        }
        if (instr->instruction == "BYTE")       // reserves bytes
        {
            // formatting whitespace
            listingFile << instr->instructionListingInfo;
            std::string op = instr->operand.substr(2, instr->operand.size() - 3);
            int object_code_padding = std::max(0, 34 - (int)instr->instructionListingInfo.length()); 
            std::string object_code_whitespace(object_code_padding, ' ');
            std::string white_space = "                 ";
            if (instr->operand[1] == 'C')       // if 'C', convert to ASCII values and print
            {
                listingFile <<  object_code_whitespace << white_space;
                for (char c : op)
                {
                    listingFile << std::hex << std::uppercase << static_cast<int>(c);
                }
                listingFile << std::endl;
            }
            else        // if 'X', directly print
            {
                listingFile << object_code_whitespace << white_space << op << std::endl;
            }

            continue;
        }
        if (instr->instruction[0] == '*')      // check for '*' 
        {
            // whitespace formatting
            std::string op = instr->operand.substr(3, instr->operand.size() - 4);
            int object_code_padding = std::max(0, 34 - (int)instr->instructionListingInfo.length());
            std::string object_code_whitespace(object_code_padding, ' ');
            std::string white_space = "                 ";

            // check for 'C'
            if (instr->operand[1] == 'C')
            {
                // if 'C', convert to ASCII values and print
                listingFile << instr->instructionListingInfo << object_code_whitespace << white_space;
                for (char c : op)
                {
                    listingFile << std::hex << std::uppercase << static_cast<int>(c);
                }
                listingFile << std::endl;
            }
            else
            {
                // if 'X', directly print
                listingFile << instr->instructionListingInfo << object_code_whitespace << white_space << op << std::endl;
            }
            continue;
        }
        // check if instruction is in the assembler directives
        if (assembler_directives.find(instr->instruction) != assembler_directives.end())
        {
            // Do something with assembler directives
            listingFile << instr->instructionListingInfo << std::endl;
        }
        else{
            int object_code_padding = std::max(0, 34 - (int)instr->instructionListingInfo.length()); // Change to 34
            std::string object_code_whitespace(object_code_padding, ' ');
            std::string white_space = "                 ";
            
            // format 1 check
            if (opcodeTable[instr->instruction].second == 1)
            {
                listingFile << instr->instructionListingInfo << object_code_whitespace << white_space << formatOneOpcode(instr) << std::endl;
            }
            // format 2 check
            else if (opcodeTable[instr->instruction].second == 2)
            {
                listingFile << instr->instructionListingInfo << object_code_whitespace << white_space << formatTwoOpcode(instr) << std::endl;
            }
            // format 3 check
            else if (opcodeTable[instr->instruction].second == 3)
            {
                listingFile << instr->instructionListingInfo << object_code_whitespace << white_space << formatThreeOpcode(instr, base) << std::endl;
            }
            // format 4 check
            else if (opcodeTable[instr->instruction].second == 4)
            {
                listingFile << instr->instructionListingInfo << object_code_whitespace << white_space << formatFourOpcode(instr) << std::endl;
            }
            // else does not exist error
            else
            {
                std::cerr << "Error: Unknown instruction format for " << instr->instruction << std::endl;
            }
        }
        
    }
}

// format1, directly prints
std::string formatOneOpcode(Instruction *instr){
    return opcodeTable[instr->instruction].first;
}

// format2 logic
std::string formatTwoOpcode(Instruction *instr){
    //TODO: add edge cases
    std::string base = opcodeTable[instr->instruction].first;
    std::vector<std::string> res;
    std::stringstream ss(instr->operand);
    std::string token;

    // split registers based on ','
    while (std::getline(ss, token, ','))
    {
        res.push_back(token);
    }

    int r1 = registerTable[res[0]];
    int r2 = 0; // default if not needed

    // Some Format 2 instructions only use r1
    if (res.size() > 1)
    {
        r2 = registerTable[res[1]];
    }

    std::stringstream output;
    output << base << r1 << r2;
    return output.str();
}

// format3 logic
std::string formatThreeOpcode(Instruction *instr, std::string base){
    int n, i, x, b, p, e; // e always 0 in F3

    n = 0;
    i = 0;
    x = 0;
    b = 0;
    p = 0;
    e = 0;
    int address = 0;

    std::string operand = instr->operand;
    std::string label = operand;

    // check RSUB routine
    if (instr->instruction == "RSUB")
    {
        int opcode = (std::stoi(opcodeTable["RSUB"].first, nullptr, 16) & 0xFC) | 0x03;
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setfill('0');
        ss << std::setw(2) << opcode << "0000";
        return ss.str();
    }
    // check immediate
    if (operand[0] == '#'){
        n = 0;
        i = 1;
        operand = operand.substr(1);
    }
    // check indireect
    else if (operand[0] == '@'){
        n = 1;
        i = 0;
        operand = operand.substr(1);
    }
    // else neither, and we use XE therefore n=i=1
    else{
        n = 1;
        i = 1;
    }

    // check for indexed addressing
    size_t commaPos = operand.find(',');
    if (commaPos != std::string::npos){
        x = 1;
        std::string indexRegister = operand.substr(commaPos + 1);
        if (indexRegister != "X"){
            std::cout << "ERROR, invaid usage of INDEXED addressing at :" << indexRegister << std::endl;
            exit(1);
        }
        label = operand.substr(0, commaPos);
    }
    else {
        label = operand;
    }

    // if we are using immediate, then label can be turned into an integer
    bool isImmediate = false;
    try{
        address = std::stoi(label);
        isImmediate = true;
        b = 0; p = 0;
    }
    catch (std::invalid_argument &){
        isImmediate = false;
    }

    // if stoi failed, then we check for base and PC relative
    if (!isImmediate){
        if(label.empty()){
            return "";
        }
        int targetAddress;
        bool isLiteral = false;
        // check label size
        if(label.size() > 4 && label[0] == '='){
            std::string op = instr->operand.substr(3, (instr->operand.size() - 4));
            // check symtab
            if (symbolTable.find(op) == symbolTable.end())
            {
                std::cout << "ERROR: Label '" << label << "' not found in symbol table" << std::endl;
                exit(1);
            }
            targetAddress = symbolTable[op];
            isLiteral = true;
        }
        // another symtab check
        else if (symbolTable.find(label) == symbolTable.end()) {
            std::cout << "ERROR: Label '" << label << "' not found in symbol table" << std::endl;
            exit(1);   
        }
        // check if is a literal, update target address to the label address
        if(!isLiteral){
            targetAddress = symbolTable[label];
        }
        // PC relative addressing
        int pcDisp = targetAddress - (instr->address + 3);
        if (pcDisp >= -2048 && pcDisp <= 2047){
            b = 0;
            p = 1;
            address = pcDisp & 0xFFF;
        }
        // Base relative addressing
        else{
            int baseDisp = targetAddress - symbolTable[base];
            if (baseDisp >= 0 && baseDisp <= 4095){
                b = 1;
                p = 0;
                address = baseDisp;
            }
            // does not satisfy either requirements, error and exit
            else {
                std::cout << "ERROR: address for " << label << " out of bounds" << std::endl;
                exit(1);
            }
        }
    }

    // compile flags and disp to make the opcode
    int baseOpcode = std::stoi(opcodeTable[instr->instruction].first, nullptr, 16);
    int opcode = (baseOpcode & 0xFC) | (n << 1) | i;
    int flags = (x << 3) | (b << 2) | (p << 1) | e;
    int byte2 = (flags << 4) | ((address >> 8) & 0x0F);
    int byte3 = address & 0xFF;

    // whitespace formatting
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');
    ss << std::setw(2) << opcode;
    ss << std::setw(2) << byte2;
    ss << std::setw(2) << byte3;

    return ss.str();
}

// format4 logic
// similar to format3, no base or PC relative checks
std::string formatFourOpcode(Instruction *instr)
{
    int n, i, x, b, p, e; // e always 0 in F3

    n = 0;
    i = 0;
    x = 0;
    b = 0;
    p = 0;
    e = 1;

    std::stringstream ss;
    ss << std::hex << std::setfill('0'); // pad with 0s

    // convert instruction into HEX
    int baseValue = std::stoi(opcodeTable[instr->instruction].first, nullptr, 16); 

    std::string operand = instr->operand;
    std::string label = operand;
    int address = 0;

    // indirect check
    if (instr->instruction[0] == '@'){ // Indirect, n = 1
        n = 1;
        i = 0;
        address = symbolTable[operand.substr(1)];
        label = operand.substr(1);
    }
    // immediate check
    else if (operand[0] == '#'){ // Immediate, i = 1
        n = 0;
        i = 1;
        address = symbolTable[operand.substr(1)];
        label = operand.substr(1);
    }
    else{ // Otherwise, n, i = 1
        n = 1;
        i = 1;
        address = symbolTable[operand];
    }

    // look for ',' indicating indeXed
    if (operand != "" && operand.find(',') != std::string::npos){    // x
        x = 1;
        
        std::vector<std::string> res;
        std::stringstream operand_index(instr->operand);
        std::string token;
        while (std::getline(operand_index, token, ','))
        {
            res.push_back(token);
        }
        std::string label = res[0];

        address = symbolTable[res[0]];
    }

    // check if stoi works for label
    bool isImmediate = false;
    try{
        address = std::stoi(label);
        isImmediate = true;
        b = 0; p = 0;
    }
    catch (std::invalid_argument &){
        isImmediate = false;
    }

    // target address checking
    int targetAddress;
    if (!isImmediate){
        if(label.empty()){
            return "";
        }
        
        bool isLiteral = false;
        // ensure size is correct
        if(label.size() > 4 && label[0] == '='){
            std::string op = instr->operand.substr(3, (instr->operand.size() - 4));
            
            if (symbolTable.find(op) == symbolTable.end())
            {
                std::cout << "ERROR: Label '" << label << "' not found in symbol table" << std::endl;
                exit(1);
            }
            // define targetAddress based on symtab
            targetAddress = symbolTable[op];
            isLiteral = true;
        }
        else if (symbolTable.find(label) == symbolTable.end()) {
            std::cout << "ERROR: Label '" << label << "' not found in symbol table" << std::endl;
            exit(1);   
        }
        if(!isLiteral){
            targetAddress = symbolTable[label];
        }        
    }

    // compile flags and disp to create opcode
    std::string operand_copy = operand;

    int opcode = (baseValue & 0xFC) | (n << 1) | i; 
    ss << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << opcode;
    int flags = (x << 3) | (b << 2) | (p << 1) | e; 

    // whitespace formatting
    ss << std::setw(1) << std::hex << flags;
    ss << std::setw(5) << std::setfill('0') << std::hex << (targetAddress & 0xFFFFF);

    return ss.str();
}