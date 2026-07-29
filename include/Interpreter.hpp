#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "AST.hpp"
#include "SymbolTable.hpp"

class Interpreter {
private:
    SymbolTable env;
    RuntimeVal evaluate(ASTNode* node);

public:
    void execute(BlockASTNode* program);
};

#endif 