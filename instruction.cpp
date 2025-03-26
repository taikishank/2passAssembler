#include "instruction.h"


// Constructor
Instruction::Instruction(std::string lbl, std::string instr, std::string opd)
    : label(lbl), instruction(instr), operand(opd){}

void Instruction::writeToListing(std::ofstream &listingFile) const {
    if (!listingFile){
        std::cout << "Listing file is not open! Open that shit up dumbass!" << std::endl;
        return;
    }

    if (!label.empty()){
        listingFile << label << "\t" << instruction << "\t" << operand << std::endl;
    }
    else{
        listingFile << "\t" << instruction << "\t" << operand << std::endl; // formatting may be off, won't know until testing
    }
    return;
}

int Instruction::format_check(std::string instruction, std::string operand){
    if (instruction[0] == '+')
        return 4;
    else if (format2_instructions.find(instruction) != format2_instructions.end())
        return 2;
    else if (instruction == "RESB")
        return stoi(operand);
    else if (instruction == "RESW")
        return stoi(operand) * 3;
    else if (instruction == "BYTE")
        if (operand[0] == 'C')
            return operand.size() - 3;
        else if (operand[0] == 'X')
            return (operand.size() - 3) / 2;
        else{
            std::cout << "You're fucking stupid";
            exit(0);
        }
    return 3;
}