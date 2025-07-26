#include "../hpp/Value.hpp"     
#include "../hpp/Callable.hpp"  

Value::Value() : data(std::monostate{}) {}
Value::Value(double v) : data(v) {}
Value::Value(bool v) : data(v) {}
Value::Value(const std::string& v) : data(v) {}
Value::Value(std::vector<Value> v) : data(std::move(v)) {} 
Value::Value(std::shared_ptr<Callable> callable) : data(callable) {}

bool Value::isNumber() const { return std::holds_alternative<double>(data); }
bool Value::isBool() const { return std::holds_alternative<bool>(data); }
bool Value::isString() const { return std::holds_alternative<std::string>(data); }
bool Value::isNull() const { return std::holds_alternative<std::monostate>(data); }
bool Value::isArray() const { return std::holds_alternative<std::vector<Value>>(data); } 
bool Value::isCallable() const { return std::holds_alternative<std::shared_ptr<Callable>>(data); } 

double Value::asNumber() const {
    if (!isNumber()) throw std::runtime_error("Value is not a number.");
    return std::get<double>(data);
}
bool Value::asBool() const {
    if (!isBool()) throw std::runtime_error("Value is not a boolean.");
    return std::get<bool>(data);
}
const std::string& Value::asString() const {
    if (!isString()) throw std::runtime_error("Value is not a string.");
    return std::get<std::string>(data);
}
const std::vector<Value>& Value::asArray() const {
    if (!isArray()) throw std::runtime_error("Value is not an array.");
    return std::get<std::vector<Value>>(data);
}
std::vector<Value>& Value::asArrayMutable() {
    if (!isArray()) throw std::runtime_error("Value is not an array.");
    return std::get<std::vector<Value>>(data);
}
std::shared_ptr<Callable> Value::asCallable() const {
    if (!isCallable()) throw std::runtime_error("Value is not a callable function.");
    return std::get<std::shared_ptr<Callable>>(data);
}

std::string Value::toString() const {
    if (isNumber()) return std::to_string(asNumber());
    if (isBool()) return asBool() ? "True" : "False";
    if (isString()) return asString(); 
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
    if (isCallable()) return asCallable()->toString(); 
    return "<unknown type>"; 
}

bool Value::operator==(const Value& other) const {
    return data == other.data; 
}
bool Value::operator!=(const Value& other) const {
    return !(*this == other);
}
