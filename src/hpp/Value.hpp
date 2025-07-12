#pragma once 

#include <variant>   
#include <string>    
#include <vector>    
#include <iostream>  
#include <stdexcept> 



class Value {
public:

    std::variant<double, bool, std::string, std::monostate, std::vector<Value>> data;

    Value() : data(std::monostate{}) {}

    Value(double v) : data(v) {}
    Value(bool v) : data(v) {}
    Value(const std::string& v) : data(v) {}
    Value(std::vector<Value> v) : data(std::move(v)) {} 

    bool isNumber() const { return std::holds_alternative<double>(data); }
    bool isBool() const { return std::holds_alternative<bool>(data); }
    bool isString() const { return std::holds_alternative<std::string>(data); }
    bool isNull() const { return std::holds_alternative<std::monostate>(data); }
    bool isArray() const { return std::holds_alternative<std::vector<Value>>(data); } 

    double asNumber() const {
        if (!isNumber()) throw std::runtime_error("Value is not a number.");
        return std::get<double>(data);
    }
    bool asBool() const {
        if (!isBool()) throw std::runtime_error("Value is not a boolean.");
        return std::get<bool>(data);
    }
    const std::string& asString() const {
        if (!isString()) throw std::runtime_error("Value is not a string.");
        return std::get<std::string>(data);
    }
    const std::vector<Value>& asArray() const {
        if (!isArray()) throw std::runtime_error("Value is not an array.");
        return std::get<std::vector<Value>>(data);
    }
    std::vector<Value>& asArrayMutable() {
        if (!isArray()) throw std::runtime_error("Value is not an array.");
        return std::get<std::vector<Value>>(data);
    }

    std::string toString() const {
        if (isNumber()) return std::to_string(asNumber());
        if (isBool()) return asBool() ? "True" : "False";
        if (isString()) return "\"" + asString() + "\""; 
        if (isNull()) return "null";
        if (isArray()) {
            std::string result = "[";
            const auto& arr = asArray();
            for (size_t i = 0; i < arr.size(); ++i) {
                result += arr[i].toString();
                if (i < arr.size() - 1) {
                    result += ", ";
                }
            }
            result += "]";
            return result;
        }
        return "<unknown type>"; 
    }

    bool operator==(const Value& other) const {
        return data == other.data; 
    }
    bool operator!=(const Value& other) const {
        return !(*this == other);
    }
};