#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>
#include <memory>

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class NumberASTNode : public ASTNode {
public:
    int value;
    NumberASTNode(int val) : value(val) {}
};

class StringASTNode : public ASTNode {
public:
    string value;
    StringASTNode(string val) : value(val) {}
};

class IdentifierASTNode : public ASTNode {
public:
    string name;
    IdentifierASTNode(string varName) : name(varName) {}
};

class BinaryExprASTNode : public ASTNode {
public:
    string op; 
    unique_ptr<ASTNode> left;
    unique_ptr<ASTNode> right;

    BinaryExprASTNode(string operation, unique_ptr<ASTNode> lhs, unique_ptr<ASTNode> rhs)
        : op(operation), left(move(lhs)), right(move(rhs)) {}
};

class OutASTNode : public ASTNode {
public:
    unique_ptr<ASTNode> expression;
    OutASTNode(unique_ptr<ASTNode> expr) : expression(move(expr)) {}
};

// Updated: in(choice) syntax
class InputASTNode : public ASTNode {
public:
    string varName;
    InputASTNode(string name) : varName(name) {}
};

class AssignmentASTNode : public ASTNode {
public:
    string varName;
    unique_ptr<ASTNode> value;
    AssignmentASTNode(string name, unique_ptr<ASTNode> val)
        : varName(name), value(move(val)) {}
};

class ArrayASTNode : public ASTNode {
public:
    vector<unique_ptr<ASTNode>> elements;
    ArrayASTNode(vector<unique_ptr<ASTNode>> elems) : elements(move(elems)) {}
};

class IndexASTNode : public ASTNode {
public:
    string arrayName;
    unique_ptr<ASTNode> index;
    IndexASTNode(string name, unique_ptr<ASTNode> idx)
        : arrayName(name), index(move(idx)) {}
};

class ArrayAssignmentASTNode : public ASTNode {
public:
    string arrayName;
    unique_ptr<ASTNode> index;
    unique_ptr<ASTNode> value;
    
    ArrayAssignmentASTNode(string name, unique_ptr<ASTNode> idx, unique_ptr<ASTNode> val)
        : arrayName(name), index(move(idx)), value(move(val)) {}
};

class BlockASTNode : public ASTNode {
public:
    vector<unique_ptr<ASTNode>> statements;
};

class CheckASTNode : public ASTNode {
public:
    unique_ptr<ASTNode> condition;
    unique_ptr<BlockASTNode> body;
    unique_ptr<BlockASTNode> elseBody; 

    CheckASTNode(unique_ptr<ASTNode> cond, unique_ptr<BlockASTNode> b)
        : condition(move(cond)), body(move(b)), elseBody(nullptr) {}
};

class LoopASTNode : public ASTNode {
public:
    string loopVar;
    unique_ptr<ASTNode> startExpr;
    unique_ptr<ASTNode> endExpr;
    unique_ptr<ASTNode> stepExpr;
    unique_ptr<BlockASTNode> body;

    LoopASTNode(string var, unique_ptr<ASTNode> start, unique_ptr<ASTNode> end, unique_ptr<ASTNode> step, unique_ptr<BlockASTNode> b)
        : loopVar(var), startExpr(move(start)), endExpr(move(end)), stepExpr(move(step)), body(move(b)) {}
};

#endif // AST_HPP
