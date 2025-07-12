#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "./Token.hpp"

inline void printIndent(int indent) {
    for (int i = 0; i < indent; ++i) std::cout << "  ";
}

class Expression {
public:
    virtual ~Expression() = default;
    virtual void print(int indent = 0) const = 0;
};

class NumberExpr : public Expression {
public:
    double value;
    NumberExpr(double v) : value(v) {}
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "NumberExpr: " << value << "\n";
    }
};

class StringExpr : public Expression {
public:
    std::string value;
    StringExpr(const std::string& v) : value(v) {}
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "StringExpr: \"" << value << "\"\n";
    }
};

class BooleanExpr : public Expression {
public:
    bool value;
    BooleanExpr(bool v) : value(v) {}
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "BooleanExpr: " << (value ? "true" : "false") << "\n";
    }
};

class VariableExpr : public Expression {
public:
    std::string name;
    VariableExpr(const std::string& v) : name(v) {}
    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "VariableExpr: " << name << "\n";
    }
};


class ArrayExpr : public Expression
{
public:

    std::vector<std::unique_ptr<Expression>> elements;
    
    ArrayExpr(std::vector<std::unique_ptr<Expression>> elems)
        : elements(std::move(elems)) {
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "ArrayExpr\n";
        for (const auto& element : elements) {
            element->print(indent + 1);
        }
    }
};



class IndexExpr : public Expression
{
public:

    std::unique_ptr<Expression> array;
    std::unique_ptr<Expression> index;

    IndexExpr(std::unique_ptr<Expression> arr, std::unique_ptr<Expression> idx)
        : array(std::move(arr)), index(std::move(idx)) {
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "IndexAccessExpr\n";
        printIndent(indent + 1);
        std::cout << "Array:\n";
        array->print(indent + 2);
        printIndent(indent + 1);
        std::cout << "Index:\n";
        index->print(indent + 2);
    }
};


class BinaryExpr : public Expression {
public:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    std::string op;

    BinaryExpr(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r, const std::string& o)
        : left(std::move(l)), right(std::move(r)), op(o) {
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "BinaryExpr: " << op << "\n";
        left->print(indent + 1);
        right->print(indent + 1);
    }
};

class UnaryExpr : public Expression {
public:
    Token op; 
    std::unique_ptr<Expression> right; 

    UnaryExpr(Token op_token, std::unique_ptr<Expression> right_expr)
        : op(std::move(op_token)), right(std::move(right_expr)) {
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "UnaryExpr: " << op.getLexeme() << "\n";
        right->print(indent + 1);
    }
};

class CallExpr : public Expression {
public:
    std::unique_ptr<Expression> callee; 
    std::vector<std::unique_ptr<Expression>> arguments;

    CallExpr(std::unique_ptr<Expression> calleeExpr, std::vector<std::unique_ptr<Expression>> args)
        : callee(std::move(calleeExpr)), arguments(std::move(args)) {
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "CallExpr:\n";
        printIndent(indent + 1);
        std::cout << "Callee:\n";
        callee->print(indent + 2);
        printIndent(indent + 1);
        std::cout << "Arguments:\n";
        for (const auto& arg : arguments) {
            arg->print(indent + 2);
        }
    }
};

class UpdateExpr : public Expression {
public:
    std::string name;
    std::string op; 
    std::unique_ptr<Expression> right; 

    UpdateExpr(const std::string& name, const std::string& op,
        std::unique_ptr<Expression> right = nullptr)
        : name(name), op(op), right(std::move(right)) {
    }
};

class GroupingExpr : public Expression {
public:
    std::unique_ptr<Expression> expression;

    GroupingExpr(std::unique_ptr<Expression> expr) : expression(std::move(expr)) {}

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "GroupingExpr:\n";
        expression->print(indent + 1);
    }
};


class Statement {
public:
    virtual ~Statement() = default;
    virtual void print(int indent = 0) const = 0;
};


class LetStatement : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> initializer;

    LetStatement(const std::string& variableName, std::unique_ptr<Expression> initExpr)
        : name(variableName), initializer(std::move(initExpr)) {
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "LetStatement: " << name << "\n";
        if (initializer) {
            initializer->print(indent + 1);
        }
    }
};


class PrintStatement : public Statement
{
public:
    std::unique_ptr<Expression> expression;
    PrintStatement(std::unique_ptr<Expression> expr) : expression(std::move(expr)) {}

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "PrintStatement:\n";
        expression->print(indent + 1);
    }
};


class ExpressionStatement : public Statement
{
public:
    std::unique_ptr<Expression> expression;
    ExpressionStatement(std::unique_ptr<Expression> expr) : expression(std::move(expr)) {}

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "ExpressionStatement:\n";
        expression->print(indent + 1);
    }
};

class UpdateStatement : public Statement {
public:
    Token nameToken; 
    Token opToken;   
    bool isPrefix;   

    UpdateStatement(Token nameToken, Token opToken, bool isPrefix)
        : nameToken(std::move(nameToken)), opToken(std::move(opToken)), isPrefix(isPrefix) {
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "UpdateStatement:\n";
        printIndent(indent + 1);
        std::cout << "Variable: " << nameToken.getLexeme() << "\n";
        printIndent(indent + 1);
        std::cout << "Operator: " << opToken.getLexeme() << "\n";
        printIndent(indent + 1);
        std::cout << "IsPrefix: " << (isPrefix ? "true" : "false") << "\n";
    }
};

class AssignmentUpdateStatement : public Statement {
public:
    Token variableNameToken; 
    Token opToken;          
    std::unique_ptr<Expression> value;

    AssignmentUpdateStatement(Token variableNameToken, Token opToken, std::unique_ptr<Expression> value)
        : variableNameToken(std::move(variableNameToken)), opToken(std::move(opToken)), value(std::move(value)) {
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "AssignmentUpdateStatement:\n";
        printIndent(indent + 1);
        std::cout << "Variable: " << variableNameToken.getLexeme() << "\n";
        printIndent(indent + 1);
        std::cout << "Operator: " << opToken.getLexeme() << "\n";
        printIndent(indent + 1);
        std::cout << "Value:\n";
        if (value) {
            value->print(indent + 2);
        }
    }
};

class BlockStatement : public Statement
{
public:
    std::vector<std::unique_ptr<Statement>> statements;

    BlockStatement(std::vector<std::unique_ptr<Statement>> stmts)
        : statements(std::move(stmts)) {
    }

    BlockStatement() = default;

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "BlockStatement:\n";
        for (const auto& stmt : statements) {
            stmt->print(indent + 1);
        }
    }
};


class IfStatement : public Statement
{
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> thenBranch;
    std::unique_ptr<Statement> elseBranch;

    IfStatement(std::unique_ptr<Expression> cond,
        std::unique_ptr<Statement> thenStmt,
        std::unique_ptr<Statement> elseStmt = nullptr)
        : condition(std::move(cond)),
        thenBranch(std::move(thenStmt)),
        elseBranch(std::move(elseStmt)) {
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "IfStatement:\n";
        printIndent(indent + 1);
        std::cout << "Condition:\n";
        condition->print(indent + 2);
        printIndent(indent + 1);
        std::cout << "ThenBranch:\n";
        thenBranch->print(indent + 2);
        if (elseBranch) {
            printIndent(indent + 1);
            std::cout << "ElseBranch:\n";
            elseBranch->print(indent + 2);
        }
    }
};


class WhileStatement : public Statement
{
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> thenBranch;

    WhileStatement(std::unique_ptr<Expression> expr, std::unique_ptr<Statement> then)
        : condition(std::move(expr)), thenBranch(std::move(then)) {
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "WhileStatement:\n";
        printIndent(indent + 1);
        std::cout << "Condition:\n";
        condition->print(indent + 2);
        printIndent(indent + 1);
        std::cout << "Body:\n";
        thenBranch->print(indent + 2);
    }
};


class FunctionStatement : public Statement {
public:
    std::string name;
    std::vector<std::string> parameters;
    std::unique_ptr<BlockStatement> body;

    FunctionStatement(std::string name,
        std::vector<std::string> params,
        std::unique_ptr<BlockStatement> body)
        : name(std::move(name)), parameters(std::move(params)), body(std::move(body)) {
    }

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "FunctionStatement: " << name << "\n";
        printIndent(indent + 1);
        std::cout << "Parameters:\n";
        for (const auto& param : parameters) {
            printIndent(indent + 2);
            std::cout << param << "\n";
        }
        printIndent(indent + 1);
        std::cout << "Body:\n";
        body->print(indent + 2);
    }
};


class ReturnStatement : public Statement
{
public:
    std::unique_ptr<Expression> expression;
    ReturnStatement(std::unique_ptr<Expression> expr) : expression(std::move(expr)) {}

    void print(int indent = 0) const override {
        printIndent(indent);
        std::cout << "ReturnStatement:\n";
        if (expression) {
            expression->print(indent + 1);
        }
    }
};
