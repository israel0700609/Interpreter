#include "../hpp/Interpreter.hpp" 
#include <iostream>   
#include <cmath>      
#include <chrono>     
#include <stdexcept>  

Value LoxFunction::call(Interpreter& interpreter, std::vector<Value> arguments) {
    std::shared_ptr<Environment> function_environment = std::make_shared<Environment>(this->closure);

    for (size_t i = 0; i < arguments.size(); ++i) {
        function_environment->define(declaration.parameters[i], arguments[i]);
    }

    try {
        interpreter.executeBlock(declaration.body->statements, function_environment);
    } catch (const Return& return_exception) { 
        return return_exception.value; 
    }
    return Value(); 
}

Interpreter::Interpreter() {
    globals = std::make_shared<Environment>();
    environment = globals; 

    globals->define("clock", Value(std::make_shared<NativeFunction>(
        "clock", 
        0,       
        [](Interpreter& interpreter, std::vector<Value> arguments) -> Value {
            return Value(static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
            ).count()) / 1000.0); 
        }
    )));
}

void Interpreter::interpret(const std::vector<std::unique_ptr<Statement>>& statements) {
    try {
        for (const auto& statement : statements) {
            execute(*statement); 
        }
    } catch (const std::runtime_error& error) {
        std::cerr << "Runtime Error: " << error.what() << std::endl;
    }
}

Value Interpreter::evaluate(const Expression& expr) {
    return expr.accept(*this); 
}

void Interpreter::execute(const Statement& stmt) {
    stmt.accept(*this); 
}

void Interpreter::executeBlock(const std::vector<std::unique_ptr<Statement>>& statements,
                              std::shared_ptr<Environment> block_environment) {
    std::shared_ptr<Environment> previous_environment = this->environment; 
    try {
        this->environment = block_environment; 

        for (const auto& statement : statements) {
            execute(*statement); 
        }
    } catch (...) {
        this->environment = previous_environment;
        throw; 
    }
    this->environment = previous_environment; 
}

void Interpreter::checkNumberOperand(const std::string& op_name, const Value& operand) {
    if (!operand.isNumber()) {
        throw std::runtime_error("Operand for '" + op_name + "' must be a number.");
    }
}

void Interpreter::checkNumberOperands(const std::string& op_name, const Value& left, const Value& right) {
    if (!left.isNumber() || !right.isNumber()) {
        throw std::runtime_error("Operands for '" + op_name + "' must be numbers.");
    }
}

void Interpreter::checkBooleanOperand(const std::string& op_name, const Value& operand) {
    if (!operand.isBool()) {
        throw std::runtime_error("Operand for '" + op_name + "' must be a boolean.");
    }
}

bool Interpreter::isTruthy(const Value& val) {
    if (val.isNull()) return false;
    if (val.isBool()) return val.asBool();
    if (val.isNumber()) return val.asNumber() != 0;
    if (val.isString()) return !val.asString().empty();
    if (val.isArray()) return !val.asArray().empty(); 
    if (val.isCallable()) return true; 
    return true; 
}

Value Interpreter::visit(const NumberExpr& expr) {
    return Value(expr.value);
}

Value Interpreter::visit(const StringExpr& expr) {
    return Value(expr.value);
}

Value Interpreter::visit(const BooleanExpr& expr) {
    return Value(expr.value);
}

Value Interpreter::visit(const VariableExpr& expr) {
    return environment->get(expr.name); 
}

Value Interpreter::visit(const ArrayExpr& expr) {
    std::vector<Value> elements_evaluated;
    elements_evaluated.reserve(expr.elements.size());
    for (const auto& element_expr : expr.elements) {
        elements_evaluated.push_back(evaluate(*element_expr));
    }
    return Value(std::move(elements_evaluated));
}

Value Interpreter::visit(const IndexExpr& expr) {
    Value array_val = evaluate(*expr.array);
    Value index_val = evaluate(*expr.index);

    if (!array_val.isArray()) {
        throw std::runtime_error("Attempted to index a non-array value.");
    }
    if (!index_val.isNumber()) {
        throw std::runtime_error("Array index must be a number.");
    }

    double raw_index = index_val.asNumber();
    if (static_cast<long long>(raw_index) != raw_index || raw_index < 0) {
        throw std::runtime_error("Array index must be a non-negative integer.");
    }
    long long index_ll = static_cast<long long>(raw_index);

    std::vector<Value>& arr_elements = array_val.asArrayMutable(); 

    if (index_ll >= arr_elements.size()) {
        throw std::runtime_error("Array index out of bounds. Index: " + std::to_string(index_ll) +
                                 ", Array size: " + std::to_string(arr_elements.size()));
    }

    return arr_elements[index_ll];
}

Value Interpreter::visit(const BinaryExpr& expr) {
    if (expr.op == "=") {
        VariableExpr* varExpr = dynamic_cast<VariableExpr*>(expr.left.get());
        if (!varExpr) {
            throw std::runtime_error("Invalid assignment target.");
        }
        Value value = evaluate(*expr.right); 
        environment->assign(varExpr->name, value); 
        return value; 
    }

    Value left = evaluate(*expr.left);
    Value right = evaluate(*expr.right);

    if (expr.op == "+") {
        if (left.isString() || right.isString()) {
            return Value(left.toString() + right.toString());
        }
        checkNumberOperands(expr.op, left, right);
        return Value(left.asNumber() + right.asNumber());
    }
    
    if (expr.op == "-" || expr.op == "*" || expr.op == "/" || expr.op == "%") {
        checkNumberOperands(expr.op, left, right);
        double left_num = left.asNumber();
        double right_num = right.asNumber();

        if (expr.op == "-") return Value(left_num - right_num);
        if (expr.op == "*") return Value(left_num * right_num);
        if (expr.op == "/") {
            if (right_num == 0) throw std::runtime_error("Division by zero.");
            return Value(left_num / right_num);
        }
        if (expr.op == "%") {
            if (right_num == 0) throw std::runtime_error("Modulo by zero.");
            if (static_cast<long long>(left_num) != left_num || static_cast<long long>(right_num) != right_num) {
                throw std::runtime_error("Modulo operands must be integers.");
            }
            return Value(static_cast<double>(static_cast<long long>(left_num) % static_cast<long long>(right_num)));
        }
    } else if (expr.op == "==") {
        return Value(left == right); 
    } else if (expr.op == "!=") {
        return Value(left != right); 
    } else if (expr.op == ">" || expr.op == ">=" || expr.op == "<" || expr.op == "<=") {
        checkNumberOperands(expr.op, left, right);
        double left_num = left.asNumber();
        double right_num = right.asNumber();
        if (expr.op == ">") return Value(left_num > right_num);
        if (expr.op == ">=") return Value(left_num >= right_num);
        if (expr.op == "<") return Value(left_num < right_num);
        if (expr.op == "<=") return Value(left_num <= right_num);
    } else if (expr.op == "&&") { 
        if (!isTruthy(left)) return Value(false); 
        return Value(isTruthy(right)); 
    } else if (expr.op == "||") { 
        if (isTruthy(left)) return Value(true); 
        return Value(isTruthy(right)); 
    }

    throw std::runtime_error("Unknown binary operator: " + expr.op);
}

Value Interpreter::visit(const UnaryExpr& expr) {
    Value right = evaluate(*expr.right);

    if (expr.op.getLexeme() == "-") { 
        checkNumberOperand("-", right);
        return Value(-right.asNumber());
    } else if (expr.op.getLexeme() == "!") { 
        return Value(!isTruthy(right));
    }

    throw std::runtime_error("Unknown unary operator: " + expr.op.getLexeme());
}

Value Interpreter::visit(const GroupingExpr& expr) {
    return evaluate(*expr.expression);
}

Value Interpreter::visit(const CallExpr& expr) {
    Value callee = evaluate(*expr.callee);

    if (!callee.isCallable()) {
        throw std::runtime_error("Can only call functions and classes. Tried to call: " + callee.toString());
    }

    std::shared_ptr<Callable> function = callee.asCallable();

    std::vector<Value> arguments;
    arguments.reserve(expr.arguments.size()); 
    for (const auto& arg_expr : expr.arguments) {
        arguments.push_back(evaluate(*arg_expr));
    }

    if (arguments.size() != function->arity()) {
        throw std::runtime_error("Expected " + std::to_string(function->arity()) +
                                 " arguments but got " + std::to_string(arguments.size()) + ".");
    }

    return function->call(*this, arguments);
}

Value Interpreter::visit(const UpdateExpr& expr) {
    Value current_val = environment->get(expr.name);
    checkNumberOperand(expr.op, current_val); 

    double num_val = current_val.asNumber();
    double new_val;

    if (expr.op == "++") {
        new_val = num_val + 1;
    } else if (expr.op == "--") {
        new_val = num_val - 1;
    } else {
        throw std::runtime_error("Unknown update operator: " + expr.op);
    }

    environment->assign(expr.name, Value(new_val));

    return Value(new_val); 
}

Value Interpreter::visit(const LetStatement& stmt) {
    Value value; 
    if (stmt.initializer) {
        value = evaluate(*stmt.initializer);
    }
    environment->define(stmt.name, value); 
    return Value(); 
}

Value Interpreter::visit(const PrintStatement& stmt) {
    Value value = evaluate(*stmt.expression);
    std::cout << value.toString() << std::endl;
    return Value();
}

Value Interpreter::visit(const ExpressionStatement& stmt) {
    evaluate(*stmt.expression);
    return Value();
}

Value Interpreter::visit(const UpdateStatement& stmt) {
    std::string var_name = stmt.nameToken.getLexeme();
    Value current_val = environment->get(var_name);
    checkNumberOperand(stmt.opToken.getLexeme(), current_val);

    double num_val = current_val.asNumber();
    double new_val;

    if (stmt.opToken.getLexeme() == "++") {
        new_val = num_val + 1;
    } else if (stmt.opToken.getLexeme() == "--") {
        new_val = num_val - 1;
    } else {
        throw std::runtime_error("Unknown update statement operator: " + stmt.opToken.getLexeme());
    }

    environment->assign(var_name, Value(new_val));

    return Value(); 
}

Value Interpreter::visit(const AssignmentUpdateStatement& stmt) {
    std::string var_name = stmt.variableNameToken.getLexeme();
    Value current_val = environment->get(var_name);

    Value right_val = evaluate(*stmt.value);

    std::string op_lexeme = stmt.opToken.getLexeme();

    if (op_lexeme == "+=") {
        // Allow string concatenation for +=
        if (current_val.isString() || right_val.isString()) {
            environment->assign(var_name, Value(current_val.toString() + right_val.toString()));
        } else {
            checkNumberOperands(op_lexeme, current_val, right_val);
            environment->assign(var_name, Value(current_val.asNumber() + right_val.asNumber()));
        }
    } else if (op_lexeme == "-=") {
        checkNumberOperands(op_lexeme, current_val, right_val);
        environment->assign(var_name, Value(current_val.asNumber() - right_val.asNumber()));
    } else if (op_lexeme == "*=") {
        checkNumberOperands(op_lexeme, current_val, right_val);
        environment->assign(var_name, Value(current_val.asNumber() * right_val.asNumber()));
    } else if (op_lexeme == "/=") {
        checkNumberOperands(op_lexeme, current_val, right_val);
        if (right_val.asNumber() == 0) throw std::runtime_error("Division by zero in assignment update.");
        environment->assign(var_name, Value(current_val.asNumber() / right_val.asNumber()));
    } else {
        throw std::runtime_error("Unknown assignment update operator: " + op_lexeme);
    }

    return Value(); 
}

Value Interpreter::visit(const BlockStatement& stmt) {
    std::shared_ptr<Environment> new_environment = std::make_shared<Environment>(this->environment);
    executeBlock(stmt.statements, new_environment); 
    return Value();
}

Value Interpreter::visit(const IfStatement& stmt) {
    Value condition_val = evaluate(*stmt.condition);
    if (isTruthy(condition_val)) {
        execute(*stmt.thenBranch);
    } else if (stmt.elseBranch) {
        execute(*stmt.elseBranch);
    }
    return Value();
}

Value Interpreter::visit(const WhileStatement& stmt) {
    while (isTruthy(evaluate(*stmt.condition))) {
        execute(*stmt.thenBranch);
    }
    return Value();
}

Value Interpreter::visit(const FunctionStatement& stmt) {
    std::shared_ptr<LoxFunction> function = std::make_shared<LoxFunction>(stmt, this->environment);
    
    environment->define(stmt.name, Value(function));
    
    return Value(); 
}

Value Interpreter::visit(const ReturnStatement& stmt) {
    Value value_to_return;
    if (stmt.expression) { 
        value_to_return = evaluate(*stmt.expression);
    } else {
        value_to_return = Value(); 
    }
    throw Return(value_to_return);
}
