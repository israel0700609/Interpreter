// src/hpp/Value.hpp
#pragma once 

#include <variant>   
#include <string>    
#include <vector>   
#include <iostream>  
#include <stdexcept> 
#include <memory>    

class Callable; 

class Value {
public:
    std::variant<double, bool, std::string, std::monostate, std::vector<Value>, std::shared_ptr<Callable>> data;

    Value(); 
    Value(double v);
    Value(bool v);
    Value(const std::string& v);
    Value(std::vector<Value> v); 
    Value(std::shared_ptr<Callable> callable); 

    bool isNumber() const;
    bool isBool() const;
    bool isString() const;
    bool isNull() const;
    bool isArray() const; 
    bool isCallable() const; 

    double asNumber() const;
    bool asBool() const;
    const std::string& asString() const;
    const std::vector<Value>& asArray() const;
    std::vector<Value>& asArrayMutable();
    std::shared_ptr<Callable> asCallable() const; 

    std::string toString() const; 

    bool operator==(const Value& other) const; 
    bool operator!=(const Value& other) const;
};