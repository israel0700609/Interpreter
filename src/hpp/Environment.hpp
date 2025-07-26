#pragma once 
#include <string> 
#include <map>
#include <memory>
#include "./Value.hpp"

class Environment {
public:

std::shared_ptr<Environment> enclosing;
Environment();

Environment(std::shared_ptr<Environment> enclosing_env);

void define(const std::string& name, const Value& value);

Value get(const std::string& name);

void assign(const std::string& name, const Value& value);

private:
    std::map<std::string, Value> values;
};