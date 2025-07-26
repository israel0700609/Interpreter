#include "../hpp/Environment.hpp" 
#include <stdexcept> 
#include <iostream> 


Environment::Environment() : enclosing(nullptr) {
}


Environment::Environment(std::shared_ptr<Environment> enclosing_env)
    : enclosing(enclosing_env) {
}

void Environment::define(const std::string& name, const Value& value) {

    if (values.count(name)) {
        throw std::runtime_error("Variable '" + name + "' already defined in this scope.");
    }
    values[name] = value;
}

Value Environment::get(const std::string& name) {
    if (values.count(name)) {
        return values[name];
    }

    if (enclosing) {
        return enclosing->get(name);
    }

    throw std::runtime_error("Undefined variable '" + name + "'.");
}

void Environment::assign(const std::string& name, const Value& value) {
    if (values.count(name)) {
        values[name] = value;
        return; 
    }

    if (enclosing) {
        enclosing->assign(name, value);
        return; 
    }

    throw std::runtime_error("Undefined variable '" + name + "' for assignment.");
}