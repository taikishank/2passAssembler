// Alejandro Pacheco
// Red ID: 827078376
// Edoras account: cssc0803@edoras.sdsu.edu
// CS 530, Spring 2025
// Assignment #1, File Reader
// main.cpp

#include "main.h"

int main(int argc, char **argv){

    if (argc < 2){
        std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
        return ERROR_RETURN_CODE;
    }
   
    std::vector <std::string> file_paths;
    for (int i = 1; i < argc; i++){
        file_paths.push_back(argv[i]);
    }

    for(std::string curr_file: file_paths){
        FILE *fp = fopen(curr_file.c_str(), "r"); // Open the file
        if (fp == nullptr){
            std::cerr << "Unable to open " << curr_file << std::endl;
            return ERROR_RETURN_CODE;
        }
    }

    return RETURN_CODE; // Return 0 indicating file finished printing
}
