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
    std::string base = "";

    for(int i = 0; i < instructionList.size(); i++){
        Instruction *instr = instructionList[i];
        if(instr->instruction == "."){
            listingFile << instr->instructionListingInfo << std::endl;
            continue;
        }
        if (instr->instruction == "NOBASE"){
            base = "";
            listingFile << instr->instructionListingInfo << std::endl;
            continue;
        }
        if (instr->instruction == "BASE"){
            base = instr->operand;
            listingFile << instr->instructionListingInfo << std::endl;
            continue;
        }
        if(instr->instruction == "END"){
            listingFile <<  instr->instructionListingInfo << std::endl;
            continue;
        }
        if (assembler_directives.find(instr->instruction) != assembler_directives.end())
        {
            // Do something with assembler directives
            listingFile << instr->instructionListingInfo << std::endl;
        }
        else{ 
            std::cout << "Instruction: " << instr->instruction << std::endl;
            
            std::cout << "Instruction Listing Info: " << instr->instructionListingInfo << std::endl;
            std::cout << std::endl;

            int object_code_padding = std::max(0, 36 - (int)instr->instructionListingInfo.length()); // Change to 34
            std::string object_code_whitespace(object_code_padding, ' ');
            std::string white_space = "                 ";

            if (opcodeTable[instr->instruction].second == 1)
            {
                listingFile << instr->instructionListingInfo << object_code_whitespace << white_space << formatOneOpcode(instr) << std::endl;
            }
            else if (opcodeTable[instr->instruction].second == 2)
            {
                listingFile << instr->instructionListingInfo << object_code_whitespace << white_space << formatTwoOpcode(instr) << std::endl;
            }
            else if (opcodeTable[instr->instruction].second == 3)
            {
                listingFile << instr->instructionListingInfo << object_code_whitespace << white_space << formatThreeOpcode(instr, base) << std::endl;
            }
            else if (opcodeTable[instr->instruction].second == 4)
            {
                listingFile << instr->instructionListingInfo << object_code_whitespace << white_space << formatFourOpcode(instr) << std::endl;
            }
            else
            {
                std::cerr << "Error: Unknown instruction format for " << instr->instruction << std::endl;
            }
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

std::string formatThreeOpcode(Instruction *instr, std::string base){
    int n, i, x, b, p, e; // e always 0 in F3

    n = 0;
    i = 0;
    x = 0;
    b = 0;
    p = 0;
    e = 0;
    int address = 0;

    if(symbolTable.find(instr->operand) != symbolTable.end()){
        address = symbolTable[instr->operand] - (instr->address + 3);
        
        if (address < -2048 || address > 2047){
            b = 1;
            p = 0;
            address = symbolTable[instr->operand] - symbolTable[base];
            std::cout << "symbolTable[base] =" << symbolTable[base] << std::endl;
        }
        else{
            b = 0;
            p = 1;
            address &= 0xFFF;
        }
    }


    int baseValue = std::stoi(opcodeTable[instr->instruction].first, nullptr, 16);
    if (instr->instruction[0] == '@')
    { // Indirect, n = 1
        n = 1;
        i = 0;
        if (base == instr->operand.substr(1, instr->operand.size())){
            b = 1;
            p = 0;
            address = symbolTable[instr->operand.substr(1, instr->operand.size())];
        }
    }
    else if (instr->operand[0] == '#'){ // Immediate, i = 1
        n = 0;
        i = 1;
        if (base == instr->operand.substr(1, instr->operand.size())){
            b = 1;
            p = 0;
            address = symbolTable[instr->operand.substr(1, instr->operand.size())];
        }
    }
    else{ // Otherwise, n, i = 1
        n = 1;
        i = 1;
    }
    if (base == instr->operand){
        b = 1;
        p = 0;
        address = symbolTable[instr->operand];
    }

    // look for ',' indicating indeXed
    if (instr->operand != "" && instr->operand.find(',') != std::string::npos){    // x
        x = 1;
        std::vector<std::string> res;
        std::stringstream operand_index(instr->operand);
        std::string token;
        while (std::getline(operand_index, token, ','))
        {
            res.push_back(token);
        }
        std::string label = res[0];

        address = symbolTable[res[0]] - registerTable[res[1]];

        if (address < -2048 || address > 2047)
        {
            b = 1;
            p = 0;
        }
        else
        {
            b = 0;
            p = 1;
            address &= 0xFFF;
        }


        if (symbolTable.find(label) != symbolTable.end()){
            p = 1;
            address = symbolTable[label] - (instr->address + 3);
            address &= 0xFFF;
        }
    }

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
    }
        
    
        
    //std::cout << instr->instruction << std::endl;
    //std::cout << "Disp: " << std::hex << address << std::endl;

    std::stringstream ss;
    ss << std::hex << std::setfill('0'); // pad with 0s

    int opcode = (baseValue & 0xFC) | (n << 1) | i;
    ss << std::setw(2) << std::uppercase << opcode;

    int flags = (x << 3) | (b << 2) | (p << 1) | e;
    int byte2 = (flags << 4) | ((address >> 8) & 0x0F);
    ss << std::setw(2) << std::uppercase << byte2;

    int byte3 = address & 0xFF;
    ss << std::setw(2) << std::uppercase << byte3;

    //std:: cout << baseValue << std::endl;
    //std::cout << instr->instruction << " nixbpe: " << n << " " << i << " " << x << " " << b << " " << p << " " << e << " " << std::endl;

    //std::cout << ss.str() << std::endl;
    //std::cout << "" << std::endl;

    return ss.str();
}
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

    int baseValue = std::stoi(opcodeTable[instr->instruction].first, nullptr, 16); // may need to move
    std::cout << instr->instruction << "  " << baseValue << std::endl;

    std::string operand = instr->operand;
    int address = 0;

    if (instr->instruction[0] == '@'){ // Indirect, n = 1
        n = 1;
        i = 0;
        address = symbolTable[operand.substr(1, operand.size())];
    }
    else if (operand[0] == '#'){ // Immediate, i = 1
        n = 0;
        i = 1;
        address = symbolTable[operand.substr(1, operand.size())];
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

    std::string operand_copy = operand;

    int opcode = (baseValue & 0xFC) | (n << 1) | i; 
    ss << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << opcode;

    int flags = (x << 3) | (b << 2) | (p << 1) | e; 
    ss << std::setw(1) << std::hex << flags;
    ss << std::setw(5) << std::setfill('0') << std::hex << (address & 0xFFFFF);

    return ss.str();
}