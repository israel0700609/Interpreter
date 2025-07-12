#include "../hpp/Lexer.hpp"
#include <stdexcept>
#include <cctype>   
#include <string>   
#include <map>
#include "../hpp/Token.hpp" 
#include <vector>

bool isAlpha(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool isAlphaNumeric(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

static const std::map<std::string, TokenType> keywords = {
    {"and", TokenType::AndAnd},
    {"else", TokenType::Else},
    {"False", TokenType::False},
    {"function", TokenType::Function},
    {"if", TokenType::If},
    {"let", TokenType::Let},
    {"print", TokenType::Print},
    {"return", TokenType::Return},
    {"True", TokenType::True},
    {"while", TokenType::While},
};


std::vector<Token> tokenizeLine(const std::string& line, int line_number)
{
    std::vector<Token> tokens;
    int i = 0;

    while (i < line.length()) {
        char c = line[i];

        switch (c) {
        case '(': tokens.emplace_back(TokenType::LParen, "(", line_number); i++; break;
        case ')': tokens.emplace_back(TokenType::RParen, ")", line_number); i++; break;
        case '{': tokens.emplace_back(TokenType::LBrace, "{", line_number); i++; break;
        case '}': tokens.emplace_back(TokenType::RBrace, "}", line_number); i++; break;
        case ',': tokens.emplace_back(TokenType::Comma, ",", line_number); i++; break;
        case '.': tokens.emplace_back(TokenType::Dot, ".", line_number); i++; break;
        case ';': tokens.emplace_back(TokenType::Semicolon, ";", line_number); i++; break;

        case '[': tokens.emplace_back(TokenType::LeftSqaure , "[", line_number); i++; break; 
        case ']': tokens.emplace_back(TokenType::RightSqaure, "]", line_number); i++; break;

        case '*':
            if (i + 1 < line.length() && line[i + 1] == '=') {
                tokens.emplace_back(TokenType::StarEqual, "*=", line_number);
                i += 2;
            }
            else {
                tokens.emplace_back(TokenType::Star, "*", line_number);
                i++;
            }
            break;

        case '+':
            if (i + 1 < line.length()) {
                if (line[i + 1] == '=') {
                    tokens.emplace_back(TokenType::PlusEqual, "+=", line_number);
                    i += 2;
                    break;
                }
                else if (line[i + 1] == '+') {
                    tokens.emplace_back(TokenType::PlusPlus, "++", line_number);
                    i += 2;
                    break;
                }
            }
            tokens.emplace_back(TokenType::Plus, "+", line_number); 
            i++;
            break;

        case '-':
            if (i + 1 < line.length()) {
                if (line[i + 1] == '-') {
                    tokens.emplace_back(TokenType::MinusMinus, "--", line_number);
                    i += 2;
                    break;
                }
                else if (line[i + 1] == '=') {
                    tokens.emplace_back(TokenType::MinusEqual, "-=", line_number);
                    i += 2;
                    break;
                }
            }
            tokens.emplace_back(TokenType::Minus, "-", line_number);
            i++;
            break;

        case '/':
            if (i + 1 < line.length()) {
                if (line[i + 1] == '=') {
                    tokens.emplace_back(TokenType::SlashEqual, "/=", line_number);
                    i += 2;
                    break;
                }
                else if (line[i + 1] == '/') {
                    i = line.length();
                    break; 
                }
            }
            tokens.emplace_back(TokenType::Slash, "/", line_number);
            i++;
            break;

        case '=':
            if (i + 1 < line.length() && line[i + 1] == '=') {
                tokens.emplace_back(TokenType::EqualEqual, "==", line_number);
                i += 2;
            }
            else {
                tokens.emplace_back(TokenType::Equal, "=", line_number);
                i++;
            }
            break;

        case '!':
            if (i + 1 < line.length() && line[i + 1] == '=') {
                tokens.emplace_back(TokenType::BangEqual, "!=", line_number);
                i += 2;
            }
            else {
                tokens.emplace_back(TokenType::Bang, "!", line_number);
                i++;
            }
            break;

        case '>':
            if (i + 1 < line.length() && line[i + 1] == '=') {
                tokens.emplace_back(TokenType::GreaterEqual, ">=", line_number);
                i += 2;
            }
            else {
                tokens.emplace_back(TokenType::Greater, ">", line_number);
                i++;
            }
            break;

        case '<':
            if (i + 1 < line.length() && line[i + 1] == '=') {
                tokens.emplace_back(TokenType::LessEqual, "<=", line_number);
                i += 2;
            }
            else {
                tokens.emplace_back(TokenType::Less, "<", line_number);
                i++;
            }
            break;

        case '&':
            if (i + 1 < line.length() && line[i + 1] == '&') {
                tokens.emplace_back(TokenType::AndAnd, "&&", line_number);
                i += 2;
            }
            else {
                throw std::runtime_error("Unexpected character '&' at line " + std::to_string(line_number) + ". Expected '&&'.");
            }
            break;

        case '|':
            if (i + 1 < line.length() && line[i + 1] == '|') {
                tokens.emplace_back(TokenType::OrOr, "||", line_number);
                i += 2;
            }
            else {
                throw std::runtime_error("Unexpected character '|' at line " + std::to_string(line_number) + ". Expected '||'.");
            }
            break;

           
        case ' ':
        case '\t':
        case '\r': 
            i++; 
            break;

          
        case '%':
            tokens.emplace_back(TokenType::Modulo, "%", line_number);
            i++;
            break;

            
        case '"': {
            i++; 
            int start_string = i;
            while (i < line.length() && line[i] != '"') {
                if (line[i] == '\n') { 
                    throw std::runtime_error("Unterminated string literal (newline) at line " + std::to_string(line_number));
                }
                i++;
            }
            if (i == line.length()) { 
                throw std::runtime_error("Unterminated string literal at line " + std::to_string(line_number));
            }
            tokens.emplace_back(TokenType::String, line.substr(start_string, i - start_string), line_number);
            i++; 
            break;
        }

                
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            int start_num = i;
            while (i < line.length() && std::isdigit(static_cast<unsigned char>(line[i]))) {
                i++;
            }
            if (i < line.length() && line[i] == '.') { 
                i++;
                while (i < line.length() && std::isdigit(static_cast<unsigned char>(line[i]))) {
                    i++;
                }
            }
            tokens.emplace_back(TokenType::Number, line.substr(start_num, i - start_num), line_number);
            break; 
        }

        default:
            if (isAlpha(c)) { 
                int start_id = i;
                while (i < line.length() && isAlphaNumeric(line[i])) {
                    i++;
                }
                std::string text = line.substr(start_id, i - start_id);

                auto it = keywords.find(text);
                if (it != keywords.end()) {
                    tokens.emplace_back(it->second, text, line_number);
                }
                else {
                    tokens.emplace_back(TokenType::Identifier, text, line_number); 
                }
            }
            else { 
                throw std::runtime_error("Unexpected character '" + std::string(1, c) + "' at line " + std::to_string(line_number));
            }
            break; 
        }
    }
    return tokens;
}