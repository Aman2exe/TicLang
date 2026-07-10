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
    std::string value;
    StringASTNode(std::string val) : value(val) {}
};

class IdentifierASTNode : public ASTNode {
public:
    std::string name;
    IdentifierASTNode(std::string varName) : name(varName) {}
};

class BinaryExprASTNode : public ASTNode {
public:
    std::string op; 
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryExprASTNode(std::string operation, std::unique_ptr<ASTNode> lhs, std::unique_ptr<ASTNode> rhs)
        : op(operation), left(std::move(lhs)), right(std::move(rhs)) {}
};

class OutASTNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;
    OutASTNode(std::unique_ptr<ASTNode> expr) : expression(std::move(expr)) {}
};

// Updated: in(choice) syntax
class InputASTNode : public ASTNode {
public:
    std::string varName;
    InputASTNode(std::string name) : varName(name) {}
};

class AssignmentASTNode : public ASTNode {
public:
    std::string varName;
    std::unique_ptr<ASTNode> value;
    AssignmentASTNode(std::string name, std::unique_ptr<ASTNode> val)
        : varName(name), value(std::move(val)) {}
};

class ArrayASTNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> elements;
    ArrayASTNode(std::vector<std::unique_ptr<ASTNode>> elems) : elements(std::move(elems)) {}
};

class IndexASTNode : public ASTNode {
public:
    std::string arrayName;
    std::unique_ptr<ASTNode> index;
    IndexASTNode(std::string name, std::unique_ptr<ASTNode> idx)
        : arrayName(name), index(std::move(idx)) {}
};

class ArrayAssignmentASTNode : public ASTNode {
public:
    std::string arrayName;
    std::unique_ptr<ASTNode> index;
    std::unique_ptr<ASTNode> value;
    
    ArrayAssignmentASTNode(std::string name, std::unique_ptr<ASTNode> idx, std::unique_ptr<ASTNode> val)
        : arrayName(name), index(std::move(idx)), value(std::move(val)) {}
};

class BlockASTNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;
};

class CheckASTNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<BlockASTNode> body;
    std::unique_ptr<BlockASTNode> elseBody; 

    CheckASTNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<BlockASTNode> b)
        : condition(std::move(cond)), body(std::move(b)), elseBody(nullptr) {}
};

class LoopASTNode : public ASTNode {
public:
    std::string loopVar;
    std::unique_ptr<ASTNode> startExpr;
    std::unique_ptr<ASTNode> endExpr;
    std::unique_ptr<ASTNode> stepExpr;
    std::unique_ptr<BlockASTNode> body;

    LoopASTNode(std::string var, std::unique_ptr<ASTNode> start, std::unique_ptr<ASTNode> end, std::unique_ptr<ASTNode> step, std::unique_ptr<BlockASTNode> b)
        : loopVar(var), startExpr(std::move(start)), endExpr(std::move(end)), stepExpr(std::move(step)), body(std::move(b)) {}
};

#endif // AST_HPP