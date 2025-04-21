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

    for (const auto &[key, value] : symbolTable)
    {
        std::cout << key << ": " << value << std::endl;
    }

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
        if (instr->instruction == "BYTE")
        {
            listingFile << instr->instructionListingInfo;
            std::string op = instr->operand.substr(2, instr->operand.size() - 3);
            int object_code_padding = std::max(0, 34 - (int)instr->instructionListingInfo.length()); // Change to 34
            std::string object_code_whitespace(object_code_padding, ' ');
            std::string white_space = "                 ";
            if (instr->operand[1] == 'C')
            {
                listingFile <<  object_code_whitespace << white_space;
                for (char c : op)
                {
                    listingFile << std::hex << std::uppercase << static_cast<int>(c);
                }
                listingFile << std::endl;
            }
            else
            {
                listingFile << object_code_whitespace << white_space << op << std::endl;
            }

            continue;
        }
        if (instr->instruction[0] == '*')
        {
            std::string op = instr->operand.substr(3, instr->operand.size() - 4);
            int object_code_padding = std::max(0, 34 - (int)instr->instructionListingInfo.length()); // Change to 34
            std::string object_code_whitespace(object_code_padding, ' ');
            std::string white_space = "                 ";

            if (instr->operand[1] == 'C')
            {
                listingFile << instr->instructionListingInfo << object_code_whitespace << white_space;
                for (char c : op)
                {
                    listingFile << std::hex << std::uppercase << static_cast<int>(c);
                }
                listingFile << std::endl;
            }
            else
            {
                listingFile << instr->instructionListingInfo << object_code_whitespace << white_space << op << std::endl;
            }
            continue;
        }
        if (assembler_directives.find(instr->instruction) != assembler_directives.end())
        {
            // Do something with assembler directives
            listingFile << instr->instructionListingInfo << std::endl;
        }
        else{
            std::cout << std::endl;
            std::cout << "Instruction: " << instr->instruction << std::endl;
            std::cout << "Instruction Listing Info: " << instr->instructionListingInfo << std::endl;
            

            int object_code_padding = std::max(0, 34 - (int)instr->instructionListingInfo.length()); // Change to 34
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

    if (instr->instruction == "RSUB")
    {
        int opcode = (std::stoi(opcodeTable["RSUB"].first, nullptr, 16) & 0xFC) | 0x03;
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setfill('0');
        ss << std::setw(2) << opcode << "0000";
        return ss.str();
    }
    if (operand[0] == '#'){
        n = 0;
        i = 1;
        operand = operand.substr(1);
    }
    else if (operand[0] == '@'){
        n = 1;
        i = 0;
        operand = operand.substr(1);
    }
    else{
        n = 1;
        i = 1;
    }

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

    bool isImmediate = false;
    try{
        address = std::stoi(label);
        isImmediate = true;
        b = 0; p = 0;
    }
    catch (std::invalid_argument &){
        isImmediate = false;
    }

    if (!isImmediate){
        if(label.empty()){
            return "";
        }
        int targetAddress;
        bool isLiteral = false;
        if(label.size() > 4 && label[0] == '='){
            std::string op = instr->operand.substr(3, (instr->operand.size() - 4));
            if (symbolTable.find(op) == symbolTable.end())
            {
                std::cout << "ERROR: Label '" << label << "' not found in symbol table" << std::endl;
                exit(1);
            }
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
        int pcDisp = targetAddress - (instr->address + 3);

        std::cout << "ADDRESSING INFO" << instr->instruction << " " << std::hex << std::uppercase << instr->label
                  << " " << std::hex << std::uppercase << targetAddress << " " << 
                  (instr->address + 3) << " " << pcDisp << std::endl;
        if (pcDisp >= -2048 && pcDisp <= 2047){
            b = 0;
            p = 1;
            address = pcDisp & 0xFFF;
        }
        else{
            int baseDisp = targetAddress - symbolTable[base];
            if (baseDisp >= 0 && baseDisp <= 4095){
                b = 1;
                p = 0;
                address = baseDisp;
            }
            else {
                std::cout << "ERROR: address for " << label << " out of bounds" << std::endl;
                exit(1);
            }
        }
    }

    int baseOpcode = std::stoi(opcodeTable[instr->instruction].first, nullptr, 16);
    int opcode = (baseOpcode & 0xFC) | (n << 1) | i;
    int flags = (x << 3) | (b << 2) | (p << 1) | e;
    int byte2 = (flags << 4) | ((address >> 8) & 0x0F);
    int byte3 = address & 0xFF;

    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');
    ss << std::setw(2) << opcode;
    ss << std::setw(2) << byte2;
    ss << std::setw(2) << byte3;

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
        address = symbolTable[operand.substr(1)];
    }
    else if (operand[0] == '#'){ // Immediate, i = 1
        n = 0;
        i = 1;
        address = symbolTable[operand.substr(1)];
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