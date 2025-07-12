#include "../hpp/Parser.hpp"
#include "../hpp/Token.hpp"
#include <stdexcept>
#include <iostream> 

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {
    std::cout << "DEBUG: Parser constructor called. Total tokens received: " << tokens.size() << std::endl;
    if (!tokens.empty()) {
        std::cout << "DEBUG: First token received in parser: '" << tokens[0].getLexeme() << "' (" << (int)tokens[0].getTokenType() << ")" << std::endl;
    }
    else {
        std::cout << "DEBUG: Parser received an empty token list." << std::endl;
    }
}

std::vector<std::unique_ptr<Statement>> Parser::parse() {
    std::cout << "DEBUG: Entering Parser::parse()" << std::endl;
    std::vector<std::unique_ptr<Statement>> statements;

    while (!isAtEnd()) {
        statements.push_back(parseStatement());
    }
    std::cout << "DEBUG: Exiting Parser::parse() successfully" << std::endl;
    return statements;
}

const Token& Parser::peek() const {
    std::cout << "DEBUG: Entering peek(). current index: " << current << ", total tokens: " << tokens.size() << std::endl;
    if (current >= tokens.size()) {
        static Token eof_token(TokenType::EndOfFile, "", tokens.empty() ? 0 : tokens.back().getLine());
        std::cout << "DEBUG: peek() returning EndOfFile token due to current index being out of bounds." << std::endl;
        return eof_token;
    }

    std::cout << "DEBUG: peek() returning token: '" << tokens[current].getLexeme() << "' (type: " << (int)tokens[current].getTokenType() << ")" << std::endl;
    return tokens[current];
}

const Token& Parser::peekNext() const {
    std::cout << "DEBUG: Entering peekNext(). current index: " << current << ", total tokens: " << tokens.size() << std::endl;
    if (current + 1 >= tokens.size()) {
        static Token eof_token(TokenType::EndOfFile, "", tokens.empty() ? 0 : tokens.back().getLine());
        std::cout << "DEBUG: peek() returning EndOfFile token due to current index being out of bounds." << std::endl;
        return eof_token;
    }

    std::cout << "DEBUG: peek() returning token: '" << tokens[current+1].getLexeme() << "' (type: " << (int)tokens[current+1].getTokenType() << ")" << std::endl;
    return tokens[current+1];
}



bool Parser::isAtEnd() const {
    std::cout << "DEBUG: Entering isAtEnd(). current index: " << current << ", total tokens: " << tokens.size() << std::endl;
    bool atEnd = current >= tokens.size() || tokens[current].getTokenType() == TokenType::EndOfFile;
    std::cout << "DEBUG: isAtEnd() result: " << (atEnd ? "true" : "false") << std::endl;
    return atEnd;
}
const Token& Parser::previous() const {
    if (current == 0)
        throw std::runtime_error("Internal Parser Error: Attempted to get previous token at start of stream.");
    return tokens[current - 1];
}


void Parser::advance() {
    if (!isAtEnd())
        current += 1;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd())
        return type == TokenType::EndOfFile;
    return tokens[current].getTokenType() == type;
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    std::cout << "DEBUG: Consuming. Current token: '" << peek().getLexeme() << "' (" << (int)peek().getTokenType() << "). Expected type: " << (int)type << std::endl;
    if (check(type)) {
        Token tok = peek();
        advance();
        return tok;
    }

    throw std::runtime_error("Parse error: " + message + " at line " + std::to_string(peek().getLine()));
}

std::unique_ptr<Expression> Parser::parseExpression() {
    std::cout << "DEBUG: Entering parseExpression(), current token: '" << peek().getLexeme() << "'" << std::endl;
    return parseAssignment();
}

std::unique_ptr<Expression> Parser::parseAssignment() {
    std::cout << "DEBUG: Entering parseAssignment(), current token: '" << peek().getLexeme() << "'" << std::endl;
    std::unique_ptr<Expression> expr = parseLogicalOr();

    if (match({ TokenType::Equal })) {
        std::cout << "DEBUG: Matched Equal in Assignment, current token: '" << peek().getLexeme() << "'" << std::endl;
        Token equals = previous();
        std::unique_ptr<Expression> value = parseAssignment();

        if (auto varExpr = dynamic_cast<VariableExpr*>(expr.get())) {
            return std::make_unique<BinaryExpr>(
                std::make_unique<VariableExpr>(varExpr->name),
                std::move(value),
                equals.getLexeme()
            );
        }

        throw std::runtime_error("Invalid assignment target at line " + std::to_string(equals.getLine()));
    }
    std::cout << "DEBUG: Exiting parseAssignment(), current token: '" << peek().getLexeme() << "'" << std::endl;
    return expr;
}

std::unique_ptr<Expression> Parser::parseLogicalOr() {
    std::cout << "DEBUG: Entering parseLogicalOr(), current token: '" << peek().getLexeme() << "'" << std::endl;
    std::unique_ptr<Expression> expr = parseLogicalAnd();

    while (match({ TokenType::OrOr })) {
        std::cout << "DEBUG: Matched OrOr, current token: '" << peek().getLexeme() << "'" << std::endl;
        Token op = previous();
        std::unique_ptr<Expression> right = parseLogicalAnd();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op.getLexeme());
    }
    std::cout << "DEBUG: Exiting parseLogicalOr(), current token: '" << peek().getLexeme() << "'" << std::endl;
    return expr;
}

std::unique_ptr<Expression> Parser::parseLogicalAnd() {
    std::cout << "DEBUG: Entering parseLogicalAnd(), current token: '" << peek().getLexeme() << "'" << std::endl;
    std::unique_ptr<Expression> expr = parseEquality();

    while (match({ TokenType::AndAnd })) {
        std::cout << "DEBUG: Matched AndAnd, current token: '" << peek().getLexeme() << "'" << std::endl;
        Token op = previous();
        std::unique_ptr<Expression> right = parseEquality();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op.getLexeme());
    }
    std::cout << "DEBUG: Exiting parseLogicalAnd(), current token: '" << peek().getLexeme() << "'" << std::endl;
    return expr;
}

std::unique_ptr<Expression> Parser::parseEquality() {
    std::cout << "DEBUG: Entering parseEquality(), current token: '" << peek().getLexeme() << "'" << std::endl;
    std::unique_ptr<Expression> expr = parseComparison();

    while (match({ TokenType::EqualEqual, TokenType::BangEqual })) {
        std::cout << "DEBUG: Matched EqualEqual or NotEqual, current token: '" << peek().getLexeme() << "'" << std::endl;
        Token op = previous();
        std::unique_ptr<Expression> right = parseComparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op.getLexeme());
    }
    std::cout << "DEBUG: Exiting parseEquality(), current token: '" << peek().getLexeme() << "'" << std::endl;
    return expr;
}

std::unique_ptr<Expression> Parser::parseComparison() {
    std::cout << "DEBUG: Entering parseComparison(), current token: '" << peek().getLexeme() << "'" << std::endl;
    std::unique_ptr<Expression> expr = parseTerm();

    while (match({ TokenType::Greater, TokenType::GreaterEqual, TokenType::Less, TokenType::LessEqual })) {
        std::cout << "DEBUG: Matched Comparison op, current token: '" << peek().getLexeme() << "'" << std::endl;
        Token op = previous();
        std::unique_ptr<Expression> right = parseTerm();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op.getLexeme());
    }
    std::cout << "DEBUG: Exiting parseComparison(), current token: '" << peek().getLexeme() << "'" << std::endl;
    return expr;
}

std::unique_ptr<Expression> Parser::parseTerm() {
    std::cout << "DEBUG: Entering parseTerm(), current token: '" << peek().getLexeme() << "'" << std::endl;
    std::unique_ptr<Expression> expr = parseFactor();

    while (match({ TokenType::Plus, TokenType::Minus })) {
        std::cout << "DEBUG: Matched Plus or Minus, current token: '" << peek().getLexeme() << "'" << std::endl;
        Token op = previous();
        std::unique_ptr<Expression> right = parseFactor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op.getLexeme());
    }
    std::cout << "DEBUG: Exiting parseTerm(), current token: '" << peek().getLexeme() << "'" << std::endl;
    return expr;
}

std::unique_ptr<Expression> Parser::parseFactor() {
    std::cout << "DEBUG: Entering parseFactor(), current token: '" << peek().getLexeme() << "'" << std::endl;

    std::unique_ptr<Expression> expr = parseUnary();

    while (match({ TokenType::Star, TokenType::Slash ,TokenType::Modulo})) {
        std::cout << "DEBUG: Matched Star or Slash, current token: '" << peek().getLexeme() << "'" << std::endl;
        Token op = previous();
      
        std::unique_ptr<Expression> right = parseUnary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op.getLexeme());
    }
    std::cout << "DEBUG: Exiting parseFactor(), current token: '" << peek().getLexeme() << "'" << std::endl;
    return expr;
}

std::unique_ptr<Expression> Parser::parseUnary() {
    std::cout << "DEBUG: Entering parseUnary(), current token: '" << peek().getLexeme() << "'" << std::endl;

    if (match({ TokenType::Bang, TokenType::Minus })) {
        Token op = previous(); 

        auto right = parseUnary(); 

        if (!right) {
            throw std::runtime_error("Expected expression after unary operator at line " + std::to_string(op.getLine()));
        }

        return std::make_unique<UnaryExpr>(op, std::move(right));
    }

    return parseCall();
}




std::unique_ptr<Expression> Parser::parseCall() {
    std::cout << "DEBUG: Entering parseCall(), current token: '" << peek().getLexeme() << "'" << std::endl;
    std::unique_ptr<Expression> expr = parsePrimary();
    while (true) {
        if (match({ TokenType::LParen })) {
            std::cout << "DEBUG: Matched LParen for Call, current token: '" << peek().getLexeme() << "'" << std::endl;
            expr = finishCall(std::move(expr));
        }
        else if (match({ TokenType::LeftSqaure }))
        {
            std::cout << "DEBUG: Matched LeftSquare for index access, current token: '" << peek().getLexeme() << "'" << std::endl;
            std::unique_ptr<Expression> index = parseExpression();
            consume(TokenType::RightSqaure, "Expect ] after index.");
            expr = std::make_unique<IndexExpr>(std::move(expr), std::move(index));
        }
        else {
            break;
        }
    }
    std::cout << "DEBUG: Exiting parseCall(), current token: '" << peek().getLexeme() << "'" << std::endl;
    return expr;
}




std::unique_ptr<Expression> Parser::finishCall(std::unique_ptr<Expression> callee) {
    std::cout << "DEBUG: Entering finishCall(), current token: '" << peek().getLexeme() << "'" << std::endl;
    std::vector<std::unique_ptr<Expression>> arguments;

    if (!check(TokenType::RParen)) {
        do {
            if (arguments.size() >= 255) {
                throw std::runtime_error("Cannot have more than 255 arguments at line " + std::to_string(peek().getLine()));
            }
            arguments.push_back(parseExpression());
        } while (match({ TokenType::Comma }));
    }

    consume(TokenType::RParen, "Expect ')' after arguments.");
    std::cout << "DEBUG: Exiting finishCall(), current token: '" << peek().getLexeme() << "'" << std::endl;
    return std::make_unique<CallExpr>(std::move(callee), std::move(arguments));
}

std::unique_ptr<Expression> Parser::parsePrimary() {
    std::cout << "DEBUG: Entering parsePrimary(), current token: '" << peek().getLexeme() << "'" << std::endl;

    if (match({ TokenType::False })) {
        return std::make_unique<BooleanExpr>(false);
    }
    if (match({ TokenType::True })) {
        return std::make_unique<BooleanExpr>(true);
    }

    if (match({ TokenType::Number })) {
        double value = std::stod(previous().getLexeme()); // Convert string lexeme to double
        return std::make_unique<NumberExpr>(value);
    }
    if (match({ TokenType::LeftSqaure }))
    {
        std::vector<std::unique_ptr<Expression>> elements;
        if (!check(TokenType::RightSqaure))
        {
            do
            {
                elements.push_back(parseExpression());
            } while (match({ TokenType::Comma }));
        }
        consume(TokenType::RightSqaure, "Expect ']' after array elements.");
        return std::make_unique<ArrayExpr>(std::move(elements));
    }
    if (match({ TokenType::String })) {
        return std::make_unique<StringExpr>(previous().getLexeme());
    }

    if (match({ TokenType::Identifier })) {
        return std::make_unique<VariableExpr>(previous().getLexeme()); // VariableExpr takes string name
    }

    if (match({ TokenType::LParen })) {
        auto expr = parseExpression(); 
        if (!match({ TokenType::RParen })) {
            
            throw std::runtime_error("Expected ')' after expression at line " + std::to_string(previous().getLine()));
        }
        return std::make_unique<GroupingExpr>(std::move(expr)); 
    }

    throw std::runtime_error("Expected expression at line " + std::to_string(peek().getLine()) + ", found '" + peek().getLexeme() + "'");
}

std::unique_ptr<Statement> Parser::parseStatement() {
    std::cout << "DEBUG: Entering parseStatement(), current token: '" << peek().getLexeme() << "'" << std::endl;
    if (match({ TokenType::Print })) return parsePrintStatement();
    if (match({ TokenType::Let })) return parseLetStatement();
    if (match({ TokenType::If })) return parseIfStatement();
    if (match({ TokenType::While })) return parseWhileStatement();
    if (match({ TokenType::Return })) return parseReturnStatement();
    if (match({ TokenType::Function })) return parseFunctionStatement();
    if (check(TokenType::LBrace)) return parseBlockStatement();
    if (check(TokenType::PlusPlus) || check(TokenType::MinusMinus)) {
        return parseUpdateStatement(true);
    }
    if (check(TokenType::Identifier)) {
        size_t temp_current = current; 
        advance(); 
        if (check(TokenType::PlusPlus) || check(TokenType::MinusMinus)) {
           
            current = temp_current; 
            return parseUpdateStatement(false); 
        }
        current = temp_current; 
    }
    if (check(TokenType::Identifier)) {
        size_t temp_current = current;
        advance(); 
        if (check(TokenType::PlusEqual) || check(TokenType::MinusEqual) ||
            check(TokenType::StarEqual) || check(TokenType::SlashEqual)) {
            current = temp_current; 
            return parseAssignmentUpdateStatement();
        }
        current = temp_current; 
    }

    return parseExpressionStatement(); 
}

std::unique_ptr<Statement> Parser::parsePrintStatement() {
    std::cout << "DEBUG: Entering parsePrintStatement(), current token: '" << peek().getLexeme() << "'" << std::endl;
    auto value = parseExpression();
    consume(TokenType::Semicolon, "Expect ';' after value.");
    std::cout << "DEBUG: Exiting parsePrintStatement()" << std::endl;
    return std::make_unique<PrintStatement>(std::move(value));
}

std::unique_ptr<Statement> Parser::parseLetStatement() {
    std::cout << "DEBUG: Entering parseLetStatement(), current token: '" << peek().getLexeme() << "'" << std::endl;
    Token nameToken = consume(TokenType::Identifier, "Expect variable name after 'let'.");
    consume(TokenType::Equal, "Expect '=' after variable name.");

    auto initializer = parseExpression();

    consume(TokenType::Semicolon, "Expect ';' after variable declaration.");
    std::cout << "DEBUG: Exiting parseLetStatement()" << std::endl;
    return std::make_unique<LetStatement>(nameToken.getLexeme(), std::move(initializer));
}


std::unique_ptr<Statement> Parser::parseUpdateStatement(bool isPrefix) {
    std::cout << "DEBUG: Entering parseUpdateStatement(isPrefix=" << (isPrefix ? "true" : "false") << "), current token: '" << peek().getLexeme() << "'" << std::endl;

    Token nameToken = tokens[current];
    Token op = tokens[current];

    if (isPrefix) {
        op = consume(
            (check(TokenType::PlusPlus) ? TokenType::PlusPlus : TokenType::MinusMinus),
            "Expected '++' or '--' operator."
        );
        nameToken = consume(TokenType::Identifier, "Expected variable name after update operator.");
    }
    else {
        nameToken = consume(TokenType::Identifier, "Expected variable name before update operator.");
        op = consume(
            (check(TokenType::PlusPlus) ? TokenType::PlusPlus : TokenType::MinusMinus),
            "Expected '++' or '--' operator."
        );
    }

    consume(TokenType::Semicolon, "Expect ';' after update statement.");
    // ---------------------

    std::cout << "DEBUG: Parsed " << (isPrefix ? "prefix" : "postfix") << " update: " << (isPrefix ? op.getLexeme() : "") << nameToken.getLexeme() << (isPrefix ? "" : op.getLexeme()) << std::endl;
    return std::make_unique<UpdateStatement>(std::move(nameToken), std::move(op), isPrefix);
}
std::unique_ptr<Statement> Parser::parseAssignmentUpdateStatement() {
    std::cout << "DEBUG: Entering parseAssignmentUpdateStatement(), current token: '" << peek().getLexeme() << "'" << std::endl;

    Token variableNameToken = consume(TokenType::Identifier, "Expected variable name before assignment update operator.");
    
    Token opToken = tokens[current];
    if (match({ TokenType::PlusEqual })) {
        opToken = previous(); 
    } else if (match({ TokenType::MinusEqual })) {
        opToken = previous();
    } else if (match({ TokenType::StarEqual })) {
        opToken = previous();
    } else if (match({ TokenType::SlashEqual })) {
        opToken = previous();
    } else {
        throw std::runtime_error("Expected assignment update operator like '+=', '-=', '*=' or '/=' at line " + std::to_string(peek().getLine()) + ", found '" + peek().getLexeme() + "'");
    }

    auto expr = parseExpression(); 

    consume(TokenType::Semicolon, "Expect ';' after assignment update statement.");

    std::cout << "DEBUG: Parsed assignment update: " << variableNameToken.getLexeme() << opToken.getLexeme() << " <expr>" << std::endl;
    return std::make_unique<AssignmentUpdateStatement>(
        std::move(variableNameToken),
        std::move(opToken),
        std::move(expr)
    );
}


std::unique_ptr<Statement> Parser::parseIfStatement() {
    std::cout << "DEBUG: Entering parseIfStatement(), current token: '" << peek().getLexeme() << "'" << std::endl;
    consume(TokenType::LParen, "Expect '(' after 'if'.");
    auto condition = parseExpression();
    consume(TokenType::RParen, "Expect ')' after condition.");

    std::unique_ptr<Statement> thenBranch = parseBlockStatement();
    std::unique_ptr<Statement> elseBranch = nullptr;
    if (match({ TokenType::Else })) {
        elseBranch = parseBlockStatement();
    }
    std::cout << "DEBUG: Exiting parseIfStatement()" << std::endl;
    return std::make_unique<IfStatement>(
        std::move(condition),
        std::move(thenBranch),
        std::move(elseBranch)
    );
}

std::unique_ptr<Statement> Parser::parseWhileStatement() {
    std::cout << "DEBUG: Entering parseWhileStatement(), current token: '" << peek().getLexeme() << "'" << std::endl;
    consume(TokenType::LParen, "Expect '(' after 'while'.");
    auto condition = parseExpression();
    consume(TokenType::RParen, "Expect ')' after condition.");

    std::unique_ptr<Statement> body = parseBlockStatement(); 
    std::cout << "DEBUG: Exiting parseWhileStatement()" << std::endl;
    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

std::unique_ptr<Statement> Parser::parseReturnStatement() {
    std::cout << "DEBUG: Entering parseReturnStatement(), current token: '" << peek().getLexeme() << "'" << std::endl;
    auto returnExpression = parseExpression();
    consume(TokenType::Semicolon, "Expect ';' after return value.");
    std::cout << "DEBUG: Exiting parseReturnStatement()" << std::endl;
    return std::make_unique<ReturnStatement>(std::move(returnExpression));
}

std::unique_ptr<FunctionStatement> Parser::parseFunctionStatement() { 
    std::cout << "DEBUG: Entering parseFunctionStatement(), current token: '" << peek().getLexeme() << "'" << std::endl;
    Token nameToken = consume(TokenType::Identifier, "Expect function name.");
    std::string functionName = nameToken.getLexeme();

    consume(TokenType::LParen, "Expect '(' after function name.");

    std::vector<std::string> parameters;

    if (!check(TokenType::RParen)) {
        do {
            if (parameters.size() >= 255) {
                throw std::runtime_error("Cannot have more than 255 parameters at line " + std::to_string(peek().getLine()));
            }
            Token param = consume(TokenType::Identifier, "Expect parameter name.");
            parameters.push_back(param.getLexeme());
        } while (match({ TokenType::Comma }));
    }

    consume(TokenType::RParen, "Expect ')' after parameters.");

    std::unique_ptr<BlockStatement> body = parseBlockStatement(); 

    std::cout << "DEBUG: Exiting parseFunctionStatement()" << std::endl;
    return std::make_unique<FunctionStatement>(std::move(functionName), std::move(parameters), std::move(body));
}

std::unique_ptr<BlockStatement> Parser::parseBlockStatement() { 
    std::cout << "DEBUG: Entering parseBlockStatement(), current token: '" << peek().getLexeme() << "'" << std::endl;
    consume(TokenType::LBrace, "Expect '{' at beginning of block.");

    std::vector<std::unique_ptr<Statement>> statements;

    while (!check(TokenType::RBrace) && !isAtEnd()) {
        statements.push_back(parseStatement());
    }

    consume(TokenType::RBrace, "Expect '}' at end of block.");
    std::cout << "DEBUG: Exiting parseBlockStatement()" << std::endl;
    return std::make_unique<BlockStatement>(std::move(statements));
}

std::unique_ptr<Statement> Parser::parseExpressionStatement() {
    std::cout << "DEBUG: Entering parseExpressionStatement(), current token: '" << peek().getLexeme() << "'" << std::endl;
    
    auto expr = parseExpression();
    consume(TokenType::Semicolon, "Expect ';' after expression.");
    std::cout << "DEBUG: Exiting parseExpressionStatement()" << std::endl;
    return std::make_unique<ExpressionStatement>(std::move(expr));
}