#pragma once
#include <string>
#include <map> 

enum class TokenType {

    // Single-character tokens.
    LParen,
    RParen, 
    LBrace, 
    RBrace,
    LeftSqaure,
    RightSqaure,

    Comma, 
    Dot, 
    Minus, 
    Plus, 
    Semicolon, 
    Slash, 
    Star,
    Modulo,

    // One or two character tokens.
    Bang, 
    BangEqual,
    Equal, 
    EqualEqual,
    Greater,
    GreaterEqual,
    Less, 
    LessEqual,

    // New: Increment/Decrement and Assignment Operators
    PlusPlus,   // ++
    MinusMinus, // --
    PlusEqual,  // +=
    MinusEqual, // -=
    StarEqual,  // *=
    SlashEqual, // /=

    // Literals.
    Identifier, 
    String, 
    Number, 
    Boolean, // Boolean for true/false

    // Keywords.
    AndAnd,   // &&
    OrOr,     // ||
    Else, 
    False, 
    Function, 
    If, 
    Let, 
    Print, 
    Return, 
    True, 
    While,

    EndOfFile
};

inline std::string tokenTypeToString(TokenType type) {
    static const std::map<TokenType, std::string> typeStrings = {
        {TokenType::LParen, "LParen"}, {TokenType::RParen, "RParen"},
        {TokenType::LBrace, "LBrace"}, {TokenType::RBrace, "RBrace"},
        {TokenType::Comma, "Comma"}, {TokenType::Dot, "Dot"},
        {TokenType::Minus, "Minus"}, {TokenType::Plus, "Plus"},
        {TokenType::Semicolon, "Semicolon"}, {TokenType::Slash, "Slash"},
        {TokenType::Star, "Star"}, {TokenType::Modulo,"Modulo"},

        {TokenType::Bang, "Bang"}, {TokenType::BangEqual, "BangEqual"},
        {TokenType::Equal, "Equal"}, {TokenType::EqualEqual, "EqualEqual"},
        {TokenType::Greater, "Greater"}, {TokenType::GreaterEqual, "GreaterEqual"},
        {TokenType::Less, "Less"}, {TokenType::LessEqual, "LessEqual"},

        // New operator types
        {TokenType::PlusPlus, "PlusPlus"},
        {TokenType::MinusMinus, "MinusMinus"},
        {TokenType::PlusEqual, "PlusEqual"},
        {TokenType::MinusEqual, "MinusEqual"},
        {TokenType::StarEqual, "StarEqual"},
        {TokenType::SlashEqual, "SlashEqual"},

        {TokenType::Identifier, "Identifier"}, {TokenType::String, "String"},
        {TokenType::Number, "Number"}, {TokenType::Boolean, "Boolean"},

        {TokenType::AndAnd, "AndAnd"}, {TokenType::OrOr, "OrOr"},
        {TokenType::Else, "Else"}, {TokenType::False, "False"},
        {TokenType::Function, "Function"}, {TokenType::If, "If"},
        {TokenType::Let, "Let"}, {TokenType::Print, "Print"},
        {TokenType::Return, "Return"}, {TokenType::True, "True"},
        {TokenType::While, "While"},

        {TokenType::EndOfFile, "EndOfFile"}
    };
    auto it = typeStrings.find(type);
    if (it != typeStrings.end()) {
        return it->second;
    }
    return "Unknown";
}

class Token {
private:
    TokenType type;
    std::string lexeme;
    int line;

public:
    Token(TokenType type, std::string lexeme, int line)
        : type(type), lexeme(std::move(lexeme)), line(line) {
    }

    TokenType getTokenType() const { return type; }
    const std::string& getLexeme() const { return lexeme; }
    int getLine() const { return line; }
};