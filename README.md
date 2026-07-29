# TicLang: Custom Programming Language & Tic-Tac-Toe

## Overview
TicLang is a custom programming language built from scratch in C++[cite: 2, 11]. The project features a complete language ecosystem, including a custom lexer, parser, abstract syntax tree (AST), and interpreter[cite: 4, 5, 6, 7]. While the language is Turing-complete enough to handle basic scripts, it comes with a built-in `Source_Code.TL` file that runs a fully functional, console-based Tic-Tac-Toe game[cite: 3, 11].

## Features
* **Custom Lexer and Parser:** Converts raw source code into tokens and structures them into an Abstract Syntax Tree (AST)[cite: 7, 10, 11].
* **Interpreter:** Evaluates the AST nodes at runtime and manages execution state[cite: 9, 11].
* **Symbol Table:** Manages variable memory, supporting Numbers, Strings, and Arrays during runtime[cite: 8].
* **Error Handling:** Includes specific error outputs for undefined variables, out-of-bounds array access, and lexical/parsing errors[cite: 8, 10, 11].

## Language Syntax Highlights
Based on the `Source_Code.TL` script, the language supports the following fundamental programming concepts:
* **Variables & Arrays:** Declare arrays like `board = [" ", " "]` and update elements via index `board[choice] = turn`.
* **I/O Operations:** Take user input using `in(varName)` and print to the console using `out("text")` or `out("\n")`.
* **Conditionals:** Control flow is managed using `check (condition)` blocks, followed by `else check` or `else` blocks, and closed with `end`[cite: 3, 6].
* **Loops:** For-style loops are defined as `loop(variable, start, end, step)`[cite: 3, 4].
* **Logical & Relational Operators:** Supports operations such as `==`, `>`, `<`, `and`, and `or` for evaluating conditions[cite: 9, 10].

## Project Architecture
The C++ codebase is modularized into the following components:
* `Lexer.hpp` / `Lexer.cpp`: Reads the `.TL` source file and generates tokens (e.g., `IDENTIFIER`, `NUMBER`, `LOOP`)[cite: 6, 10].
* `Parser.hpp`: Consumes tokens and constructs the AST using recursive descent parsing[cite: 7].
* `AST.hpp`: Defines the hierarchical node structures (e.g., `BinaryExprASTNode`, `LoopASTNode`, `ArrayASTNode`)[cite: 4].
* `Interpreter.hpp` / `Interpreter.cpp`: Executes the logic defined in the AST by evaluating each node[cite: 5, 9].
* `SymbolTable.hpp`: Tracks the `RuntimeVal` of all variables, handling type checking for strings, numbers, and arrays[cite: 8].

## Getting Started

### Prerequisites
* A C++ compiler that supports C++17 (e.g., `g++`).
* `make` utility installed[cite: 2].

### Compilation
The project includes a `Makefile` to handle the build process. To compile the interpreter, run:
```bash
make
