#pragma once

#include <vector>     
#include <string>     
#include <memory>     
#include <functional> 

#include "AST.hpp"     
#include "Value.hpp"   

class Interpreter; 
class Environment; 

class Callable {
public:
    virtual Value call(Interpreter& interpreter, std::vector<Value> arguments) = 0;
    virtual int arity() const = 0;
    virtual std::string toString() const = 0;
    virtual ~Callable() = default;
};

class LoxFunction : public Callable {
public:
    const FunctionStatement& declaration; 
    std::shared_ptr<Environment> closure; 

    LoxFunction(const FunctionStatement& declaration, std::shared_ptr<Environment> closure)
        : declaration(declaration), closure(closure) {}

    Value call(Interpreter& interpreter, std::vector<Value> arguments) override; 

    int arity() const override {
        return declaration.parameters.size();
    }

    std::string toString() const override {
        return "<function " + declaration.name + ">";
    }
};

class NativeFunction : public Callable {
public:
    std::function<Value(Interpreter&, std::vector<Value>)> native_func;
    int _arity;        
    std::string _name; 

    NativeFunction(std::string name, int arity, std::function<Value(Interpreter&, std::vector<Value>)> func)
        : native_func(func), _arity(arity), _name(name) {}

    Value call(Interpreter& interpreter, std::vector<Value> arguments) override {
        return native_func(interpreter, arguments);
    }

    int arity() const override { return _arity; }
    std::string toString() const override { return "<native function " + _name + ">"; }
};