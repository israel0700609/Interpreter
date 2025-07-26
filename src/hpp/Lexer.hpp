#pragma once
#include <vector>
#include <string>
#include "./Token.hpp" 

std::vector<Token> tokenizeLine(const std::string& line, int currentLine);