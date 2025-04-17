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

void pass2(std::ofstream& listingFile, std::vector <Instruction *> instructionList)
{
    for(int i = 0; i < instructionList.size(); i++){
        Instruction *instr = instructionList[i];
        if(instr->instruction == "END"){
            listingFile << instr->instruction << " " << instr->operand << std::endl;
            continue;
        }
        if (assembler_directives.find(instr->instruction) != assembler_directives.end())
        {
            // Do something with assembler directives
            listingFile << "0000 0000" << std::endl;
        }
        else if(opcodeTable[instr->instruction].second == 1){
            listingFile << formatOneOpcode(instr) << std::endl;
        }
        else if(opcodeTable[instr->instruction].second == 2){
            listingFile << formatTwoOpcode(instr) << std::endl;
        }
        else if(opcodeTable[instr->instruction].second == 3){
            listingFile << formatThreeOpcode(instr) << std::endl;
        }
        else if(opcodeTable[instr->instruction].second == 4){
            listingFile << formatFourOpcode(instr) << std::endl;
        }
        else{
            std::cerr << "Error: Unknown instruction format for " << instr->instruction << std::endl;
        }
    }
}

std::string formatOneOpcode(Instruction *instr){
    return opcodeTable[instr->instruction].first;
}

std::string formatTwoOpcode(Instruction *instr){
    //TODO: add edge cases
    std::string base = opcodeTable[instr->instruction].first;
    std::vector<std::string> res;
    std::stringstream ss(instr->operand);
    std::string token;
    while (std::getline(ss, token, ',')){
        res.push_back(token);
    }
    int r1 = registerTable[res[0]];
    int r2 = registerTable[res[1]];
    std::stringstream output;
    output << base << r1 << r2;
    return output.str();
}

std::string formatThreeOpcode(Instruction *instr){
    int n, i, x, b, p, e; // e always 0 in F3

    n = 0;
    i = 0;
    x = 0;
    b = 0;
    p = 0;
    e = 0;
    int address = 0;

    int base = std::stoi(opcodeTable[instr->instruction].first, nullptr, 16);
    if (instr->instruction[0] == '@')
    { // Indirect, n = 1
        n = 1;
        i = 0;
    }
    else if (instr->operand[0] == '#'){ // Immediate, i = 1
        n = 0;
        i = 1;
    }
    else{ // Otherwise, n, i = 1
        n = 1;
        i = 1;
    }

    // look for ',' indicating indeXed
    if (instr->operand.find(',') != std::string::npos)    // x
        x = 1;

        std::vector<std::string> res;
        std::stringstream operand_index(instr->operand);
        std::string token;
        while (std::getline(operand_index, token, ','))
        {
            res.push_back(token);
        }
        std::string label = res[0];

        if (symbolTable.find(label) != symbolTable.end()){
            p = 1;
            address = symbolTable[label] - (instr->address + 3);

            std::cout << "Second: " << std::dec << symbolTable[label] << std::endl;
            std::cout << "First: " << std::dec << instr->address << std::endl;

            address &= 0xFFF;
            b = 0;
            p = 1;
        }


    else 
        x = 0;

    std::string operand_copy = instr->operand;
    
    if (instr->operand.find('@') != std::string::npos)
        { // Indirect
        operand_copy.erase(0, 1);
        try
        {
            std::stoi(operand_copy);
            address = std::stoi(operand_copy);
        }
        catch (std::invalid_argument &e)
        {
            if (symbolTable.find(instr->instruction) != symbolTable.end()) // If there is a valid label in the symbol table, use that address
            {
                address = symbolTable[operand_copy];
            }
            else{
                std::cerr << "Error: Invalid label " << operand_copy << std::endl;
                return "";
            }
        }
    }

    else if (instr->operand.find('#') != std::string::npos){
        operand_copy.erase(0, 1);
        if (symbolTable.find(instr->instruction) != symbolTable.end()) // If there is a valid label in the symbol table, use that address
        {
            address = symbolTable[operand_copy];
        }
        else{
            try{
                std::stoi(operand_copy);
                address = std::stoi(operand_copy);
            }
            catch (std::invalid_argument &e){
                std::cerr << "Error: Invalid label " << operand_copy << std::endl;
                return "";
            }
        }
        b = 0;
        p = 0;
        //std::cout << "disp: " << address << std::endl;
    }
        
    else if(symbolTable.find(instr->operand) != symbolTable.end()){
        address = symbolTable[instr->operand] - (instr->address + 3);

        std::cout << "Second: " << std::dec << symbolTable[instr->operand] << std::endl;
        std::cout << "First: " << std::dec << instr->address << std::endl;
        
        address &= 0xFFF;
        b = 0;
        p = 1;

    }
        
    std::cout << instr->instruction << std::endl;

    std::cout << "Disp: " << std::hex << address << std::endl;

    std::stringstream ss;
    ss << std::hex << std::setfill('0'); // pad with 0s

    int opcode = (base & 0xFC) | (n << 1) | i;
    ss << std::setw(2) << opcode;

    int flags = (x << 3) | (b << 2) | (p << 1) | e;
    int byte2 = (flags << 4) | ((address >> 8) & 0x0F);
    ss << std::setw(2) << byte2;

    int byte3 = address & 0xFF;
    ss << std::setw(2) << byte3;

    std:: cout << base << std::endl;
    std::cout << instr->instruction << " nixbpe: " << n << " " << i << " " << x << " " << b << " " << p << " " << e << " " << std::endl;

    std::cout << ss.str() << std::endl;
    std::cout << "" << std::endl;

    return ss.str();
}
std::string formatFourOpcode(Instruction *instr)
{
    return "";
}

// for every instruction in instruction list 

// generate an opcode for that instruction
// traverse through the current line, reach the end add space, insert opcode 
// Each format has its own algorithm
    // (Function)



// If we need PC:
// we check if the label is in the symbol table