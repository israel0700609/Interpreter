#pragma once 

#include "Visitor.hpp"   
#include "AST.hpp"       
#include "Value.hpp"     
#include "Environment.hpp" 
#include "Token.hpp"     
#include "Callable.hpp"  
#include "Return.hpp"    

#include <vector>
#include <map>       
#include <stdexcept> 

class Interpreter : public Visitor {
public:
    Interpreter();

    void interpret(const std::vector<std::unique_ptr<Statement>>& statements);

    Value visit(const NumberExpr& expr) override;
    Value visit(const StringExpr& expr) override;
    Value visit(const BooleanExpr& expr) override;
    Value visit(const VariableExpr& expr) override;
    Value visit(const ArrayExpr& expr) override;
    Value visit(const IndexExpr& expr) override;
    Value visit(const BinaryExpr& expr) override;
    Value visit(const UnaryExpr& expr) override;
    Value visit(const CallExpr& expr) override;
    Value visit(const UpdateExpr& expr) override;
    Value visit(const GroupingExpr& expr) override;

    Value visit(const LetStatement& stmt) override;
    Value visit(const PrintStatement& stmt) override;
    Value visit(const ExpressionStatement& stmt) override;
    Value visit(const UpdateStatement& stmt) override;
    Value visit(const AssignmentUpdateStatement& stmt) override;
    Value visit(const BlockStatement& stmt) override;
    Value visit(const IfStatement& stmt) override;
    Value visit(const WhileStatement& stmt) override;
    Value visit(const FunctionStatement& stmt) override;
    Value visit(const ReturnStatement& stmt) override;
    void executeBlock(const std::vector<std::unique_ptr<Statement>>& statements,
                      std::shared_ptr<Environment> block_environment);

private:
    std::shared_ptr<Environment> globals;
    std::shared_ptr<Environment> environment;

    Value evaluate(const Expression& expr);
    void execute(const Statement& stmt);

    void checkNumberOperand(const std::string& op_name, const Value& operand);
    void checkNumberOperands(const std::string& op_name, const Value& left, const Value& right);
    void checkBooleanOperand(const std::string& op_name, const Value& operand);
    bool isTruthy(const Value& val);
};