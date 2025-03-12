#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void pass1(const std::string &filename){
    std::ifstream file(filename);
    if (!file){
        std::cout << "Unable to open file " << filename << ". Shutting down." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)){
        std::istringstream iss(line);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word){
            words.push_back(word);
        }

        if (words.size() == 2){
            // pass symbol and value
        }
        else if (words.size() == 3){
            // pass name, symbol, value
        }
        else{
            std::cout << "ERROR: Invalid input format. Line must contain either 2 or 3 words" << std::endl;
            return;
        }
    }
}