#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Token.hpp"
#include "AST.hpp"   

class Parser
{
private:
    const std::vector<Token>& tokens;
    int current = 0;

    const Token& peek()const;
    const Token& peekNext()const;
    const Token& previous()const; 
    bool isAtEnd() const; 
    void advance(); 
    bool check(TokenType type) const; 
    bool match(std::initializer_list<TokenType> types); 
    Token consume(TokenType type, const std::string& message); 

    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parsePrintStatement();
    std::unique_ptr<Statement> parseLetStatement();
    std::unique_ptr<Statement> parseIfStatement();
    std::unique_ptr<Statement> parseWhileStatement();
    std::unique_ptr<Statement> parseReturnStatement();
    std::unique_ptr<Statement> parseUpdateStatement(bool isPrefix);
    std::unique_ptr<Statement> parseAssignmentUpdateStatement();
    std::unique_ptr<FunctionStatement> parseFunctionStatement();
    std::unique_ptr<BlockStatement> parseBlockStatement();
    std::unique_ptr<Statement> parseExpressionStatement();

    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseEquality();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parseTerm();
    std::unique_ptr<Expression> parseFactor();
    std::unique_ptr<Expression> parseUnary(); 
    std::unique_ptr<Expression> parseLogicalAnd();
    std::unique_ptr<Expression> parseLogicalOr();
    std::unique_ptr<Expression> parseAssignment();
    std::unique_ptr<Expression> parseCall();
    std::unique_ptr<Expression> finishCall(std::unique_ptr<Expression> callee);
    std::unique_ptr<Expression> parsePrimary();

public:
    Parser(const std::vector<Token>& tokens);
    std::vector<std::unique_ptr<Statement>> parse(); 
};