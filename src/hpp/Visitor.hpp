#pragma once

#include "Value.hpp" 
#include "Token.hpp" 

class NumberExpr;
class StringExpr;
class BooleanExpr;
class VariableExpr;
class ArrayExpr;
class IndexExpr;
class BinaryExpr;
class UnaryExpr;
class CallExpr;
class UpdateExpr;
class GroupingExpr;

class LetStatement;
class PrintStatement;
class ExpressionStatement;
class UpdateStatement;
class AssignmentUpdateStatement;
class BlockStatement;
class IfStatement;
class WhileStatement;
class FunctionStatement;
class ReturnStatement;

class Visitor {
public:
    virtual Value visit(const NumberExpr& expr) = 0;
    virtual Value visit(const StringExpr& expr) = 0;
    virtual Value visit(const BooleanExpr& expr) = 0;
    virtual Value visit(const VariableExpr& expr) = 0;
    virtual Value visit(const ArrayExpr& expr) = 0;      
    virtual Value visit(const IndexExpr& expr) = 0;      
    virtual Value visit(const BinaryExpr& expr) = 0;
    virtual Value visit(const UnaryExpr& expr) = 0;
    virtual Value visit(const CallExpr& expr) = 0;
    virtual Value visit(const UpdateExpr& expr) = 0;     
    virtual Value visit(const GroupingExpr& expr) = 0;

    virtual Value visit(const LetStatement& stmt) = 0;
    virtual Value visit(const PrintStatement& stmt) = 0;
    virtual Value visit(const ExpressionStatement& stmt) = 0;
    virtual Value visit(const UpdateStatement& stmt) = 0;    
    virtual Value visit(const AssignmentUpdateStatement& stmt) = 0; 
    virtual Value visit(const BlockStatement& stmt) = 0;
    virtual Value visit(const IfStatement& stmt) = 0;
    virtual Value visit(const WhileStatement& stmt) = 0;
    virtual Value visit(const FunctionStatement& stmt) = 0;
    virtual Value visit(const ReturnStatement& stmt) = 0;

    virtual ~Visitor() = default;
};