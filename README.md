# Interpreter


# 🧠 CustomLang – A Custom Programming Language (In Development)

**CustomLang** is an expression-based programming language built from scratch using C++.  
It features its own lexer, parser, AST structure, Environment, Visitor and interpreter.  
> ⚠️ Note: This project is currently **under active development** and is not production-ready.

---

## 🚀 Project Goals

- Learn and demonstrate how programming languages are built from the ground up.
- Implement the core components of a language: Lexer, Parser, AST, Visitor, Environment, Interpreter.
- Support variables, arithmetic, conditionals, loops, functions, and arrays.
- Practice modern C++ design patterns and memory management using `std::unique_ptr`, `std::variant`, etc.

---

## ✅ Current Features

- **Lexer**: Tokenizes source code into symbols (numbers, identifiers, keywords, etc.).
- **Parser**: Builds an Abstract Syntax Tree (AST) from tokens.
- **AST**: Represents expressions and statements (e.g., binary operations, variable declarations, function calls).
- **Expressions**:
  - Numbers, Booleans, Strings
  - Binary operations: `+`, `%`, `-`, `*`, `/`, `==`, `!=`, `<`, `<=`, `>` ,`>=`
  - Grouping with parentheses
  - Variable references
- **Statements**:
  - `let` declarations
  - `print` statements
  - `if` / `else` conditions
  - `while` loops
  - `return` statements
  - Blocks (`{ ... }`)
- **Functions**: Declaration and invocation with parameters
- **Arrays**: Array literals and indexing (partial support)
- **Basic Type System**: via a `Value` class (supports `double`, `bool`, `std::string`)


---

## 🧾 Token Types

### 🔹 Single-Character Tokens
- `LParen` (`(`), `RParen` (`)`), `LBrace` (`{`), `RBrace` (`}`), `LeftSqaure` (`[`), `RightSqaure` (`]`)
- `Comma` (`,`), `Dot` (`.`), `Minus` (`-`), `Plus` (`+`), `Semicolon` (`;`), `Slash` (`/`), `Star` (`*`), `Modulo` (`%`)

### 🔸 One or Two Character Tokens
- `Bang` (`!`), `BangEqual` (`!=`), `Equal` (`=`), `EqualEqual` (`==`)
- `Greater` (`>`), `GreaterEqual` (`=>`), `Less` (`<`), `LessEqual` (`<=`)
- `PlusPlus` (`++`) , `MinusMinus` (`--`), `PlusEqual` (`+=`), `MinusEqual` (`-=`), `StarEqual` (`*=`), `SlashEqual` (`/=`)

### 🔤 Literals
- `Identifier`, `String`, `Number`, `Boolean`

### 🟪 Keywords (Reserved Words)
These cannot be used as variable names: 
- `AndAnd` (`&&`), `OrOr` (`||`)
- `Else`, `False`, `Function`, `If`, `Let`, `Print`, `Return`, `True`, `While`

### 🏁 Special
- `EndOfFile`

---

---

## 📦 File Structure

| File/Folder      | Purpose |
|------------------|---------|
| `Token.hpp`       | Define token types and the token structure |
| `Lexer.hpp/cpp`     | Tokenizes raw source code |
| `Parser.hpp/cpp`    | Builds the AST from tokens |
| `AST/Expression.hpp` | Expression node definitions |
| `AST/Statement.hpp`  | Statement node definitions |
| `Value.hpp`         | Represents runtime values (e.g., numbers, strings) |
| `Interpreter.hpp/cpp` | Walks the AST and executes code (WIP) |
| `main.cpp`        | Entry point for running source files or REPL |

---

## 🛠️ Planned Features
- ✅ Variable declarations (`let`)
- ✅ Arithmetic & logical expressions
- ✅ Print statements
- ✅ Conditional statements (`if` / `else`)
- ✅ Looping with `while`
- ✅ Functions (WIP)
- ✅ Return statements
- ✅ Update expressions (`++`, `--`, `+=`, `-=`, etc.)
- ⚠️ Arrays (partial support)
- ✅ Custom value system (numbers, strings, booleans)

## ## 🚧 In Development
- [ ] Error handling with clear messages and line info
- [ ] Function scopes and closures
- [ ] Native functions (e.g., `clock()`)
- [ ] Array manipulation functions
- [ ] Type checking or inference
- [ ] Basic REPL mode

---

## 📚 Example Code (Work In Progress)

```c
let x = 10;
let y = x + 5;

if (y > 10) 
{
    print "Greater than 10";
} else 
{
    print "Smaller or equal to 10";
}

function add(a, b) 
{
    return a + b;
}

print add(3, 4);


function main()
{
    let i = 0;
    while(i < 5)
    {
        i++;
        print add(i,i+1);
    }

    return 0;
}
