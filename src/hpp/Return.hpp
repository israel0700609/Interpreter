#pragma once

#include "Value.hpp" 

class Return : public std::runtime_error {
public:
    Value value; 

    Return(Value value) : std::runtime_error(""), value(value) {}
};