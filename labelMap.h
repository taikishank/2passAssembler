#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <map>
#include <set>
#include <string>
#include <utility> // Include for std::pair

extern std::map<std::string, std::pair<std::string, int>> symbolTable; // Declaration of the map
extern std::set<std::string> assembler_directives;
#endif