#include <iostream>  
#include <fstream>   
#include <sstream>   
#include <vector>    
#include <memory>    

#include "../hpp/Token.hpp"     
#include "../hpp/Lexer.hpp"     
#include "../hpp/Parser.hpp"    
#include "../hpp/AST.hpp"       

#include "../hpp/Environment.hpp" 
#include "../hpp/Visitor.hpp"     
#include "../hpp/Callable.hpp"    
#include "../hpp/Return.hpp"      
#include "../hpp/Interpreter.hpp" 

int main() {
    std::string filename = "code.lang";

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'. Make sure it's in the same directory as the executable." << std::endl;
        return 1; 
    }

    std::string line;
    std::vector<Token> all_tokens;
    int current_line_number = 0;

    std::cout << "--- Starting Lexing ---" << std::endl;
    while (std::getline(file, line)) {
        current_line_number++;
        try {
            std::vector<Token> line_tokens = tokenizeLine(line, current_line_number);
            for (const auto& token : line_tokens) {
                all_tokens.push_back(token);
            }
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Lexing Error on line " << current_line_number << ": " << e.what() << std::endl;
            return 1; 
        }
    }
    file.close(); 

    all_tokens.emplace_back(TokenType::EndOfFile, "", current_line_number + 1);
    std::cout << "--- Lexing Finished. Total Tokens: " << all_tokens.size() << " ---" << std::endl;

    std::cout << "\n--- Starting Parsing ---" << std::endl;
    std::vector<std::unique_ptr<Statement>> statements; 
    try {
        Parser parser(all_tokens);
        statements = parser.parse();

        std::cout << "--- Parsing Finished. Statements Parsed: " << statements.size() << " ---" << std::endl;
        std::cout << "\n--- Generated AST ---" << std::endl;

        for (const auto& stmt : statements) {
            stmt->print(); 
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Parsing Error: " << e.what() << std::endl;
        return 1; 
    }

    std::cout << "\n--- Starting Interpretation ---" << std::endl;
    try {
        Interpreter interpreter;
        interpreter.interpret(statements);

        std::cout << "--- Interpretation Finished Successfully ---" << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Interpretation (Runtime) Error: " << e.what() << std::endl;
        return 1; 
    }

    std::cout << "\n--- Program Finished ---" << std::endl;

    return 0; 
}