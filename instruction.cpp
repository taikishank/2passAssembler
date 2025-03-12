#include "Instruction.h"


// Constructor
Instruction::Instruction(std::string lbl, std::string opc, std::string opd)
    : label(lbl), opcode(opc), operand(opd){}

void Instruction::writeToListing(std::ofstream &listingFile) const {
    if (!listingFile){
        std::cout << "Listing file is not open! Open that shit up dumbass!" << std::endl;
        return;
    }

    if (!label.empty()){
        listingFile << label << "\t" << opcode << "\t" << operand << std::endl;
    }
    else{
        listingFile << "\t" << opcode << "\t" << operand << std::endl; // formatting may be off, won't know until testing
    }
    return;
}
