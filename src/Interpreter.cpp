#include "Interpreter.hpp"
#include <iostream>
#include <stdexcept>

void Interpreter::execute(BlockASTNode* program){
    if(!program) return;
    for(auto& stmt : program->statements){
        evaluate(stmt.get());
    }
}

RuntimeVal Interpreter::evaluate(ASTNode* node){
    if (!node) return RuntimeVal();
    if (auto n = dynamic_cast<NumberASTNode*>(node)) return RuntimeVal(n->value);
    if (auto s = dynamic_cast<StringASTNode*>(node)) return RuntimeVal(s->value);
    if (auto id = dynamic_cast<IdentifierASTNode*>(node)) return env.getVar(id->name);
    
    if (auto arr = dynamic_cast<ArrayASTNode*>(node)){
       vector<RuntimeVal> elements;
        for (auto& el : arr->elements) {
            elements.push_back(evaluate(el.get()));
        }
        return RuntimeVal(elements);
    }
    if (auto idx = dynamic_cast<IndexASTNode*>(node)){
        RuntimeVal indexVal = evaluate(idx->index.get());
        if (indexVal.type != RuntimeVal::NUMBER) throw runtime_error("Index must be a number");
        return env.getIndex(idx->arrayName, indexVal.num);
    }

    if (auto binOp = dynamic_cast<BinaryExprASTNode*>(node)){
        RuntimeVal left = evaluate(binOp->left.get());
        RuntimeVal right = evaluate(binOp->right.get());

        if (binOp->op == "+"){
            if (left.type == RuntimeVal::NUMBER && right.type == RuntimeVal::NUMBER) return RuntimeVal(left.num + right.num);
            if (left.type == RuntimeVal::STRING || right.type == RuntimeVal::STRING) return RuntimeVal(left.toString() + right.toString());
        }
        if (binOp->op == "-"){
            if (left.type == RuntimeVal::NUMBER && right.type == RuntimeVal::NUMBER) return RuntimeVal(left.num - right.num);
            throw runtime_error("Minus only supports numbers");
        }
        if (binOp->op == "=="){
            if (left.type == RuntimeVal::NUMBER && right.type == RuntimeVal::NUMBER) return RuntimeVal(left.num == right.num ? 1 : 0);
            if (left.type == RuntimeVal::STRING && right.type == RuntimeVal::STRING) return RuntimeVal(left.str == right.str ? 1 : 0);
            return RuntimeVal(0); 
        }
        if (binOp->op == ">"){
            if (left.type == RuntimeVal::NUMBER && right.type == RuntimeVal::NUMBER) return RuntimeVal(left.num > right.num ? 1 : 0);
        }
        if (binOp->op == "<"){
            if (left.type == RuntimeVal::NUMBER && right.type == RuntimeVal::NUMBER) return RuntimeVal(left.num < right.num ? 1 : 0);
        }
        if (binOp->op == "and") return RuntimeVal((left.isTruthy() && right.isTruthy()) ? 1 : 0);
        if (binOp->op == "or") return RuntimeVal((left.isTruthy() || right.isTruthy()) ? 1 : 0);
        
        throw runtime_error("Unsupported binary operator: " + binOp->op);
    }

    if (auto assign = dynamic_cast<AssignmentASTNode*>(node)){
        RuntimeVal val = evaluate(assign->value.get());
        env.setVar(assign->varName, val);
        return val;
    }
    if (auto arrAssign = dynamic_cast<ArrayAssignmentASTNode*>(node)){
        RuntimeVal idx = evaluate(arrAssign->index.get());
        if (idx.type != RuntimeVal::NUMBER) throw runtime_error("Array index must be a number");
        RuntimeVal val = evaluate(arrAssign->value.get());
        env.setIndex(arrAssign->arrayName, idx.num, val);
        return val;
    }

    if (auto outNode = dynamic_cast<OutASTNode*>(node)){
    RuntimeVal val = evaluate(outNode->expression.get());
   string outputStr = val.toString();
    if(outputStr == "\\n") {
       cout<<endl;
    } 
    else{
       cout << outputStr;
    }
    return RuntimeVal(); 
}

    if (auto inNode = dynamic_cast<InputASTNode*>(node)){
       string inputStr;
       cin >> inputStr;
        try {
            size_t pos;
            int num =stoi(inputStr, &pos);
            if (pos == inputStr.length()){
                env.setVar(inNode->varName, RuntimeVal(num));
                return RuntimeVal();
            }
        } catch(...){} 
        env.setVar(inNode->varName, RuntimeVal(inputStr));
        return RuntimeVal();
    }

    if (auto checkNode = dynamic_cast<CheckASTNode*>(node)){
        RuntimeVal cond = evaluate(checkNode->condition.get());
        if (cond.isTruthy()){
            if (checkNode->body) execute(checkNode->body.get());
        } else if (checkNode->elseBody) {
            execute(checkNode->elseBody.get());
        }
        return RuntimeVal();
    }

    if (auto loopNode = dynamic_cast<LoopASTNode*>(node)){
        RuntimeVal startVal = evaluate(loopNode->startExpr.get());
        RuntimeVal endVal = evaluate(loopNode->endExpr.get());
        RuntimeVal stepVal = evaluate(loopNode->stepExpr.get());

        if (startVal.type!=RuntimeVal::NUMBER || endVal.type!=RuntimeVal::NUMBER || stepVal.type!=RuntimeVal::NUMBER){
            throw runtime_error("Loop parameters must be numbers");
        }
        env.setVar(loopNode->loopVar, startVal);

        while(true){
            int currentVal = env.getVar(loopNode->loopVar).num;            
            if(stepVal.num>0 && currentVal>endVal.num) break;
            if(stepVal.num<0 && currentVal<endVal.num) break;
            if(stepVal.num==0) throw runtime_error("Infinite loop: step is 0");
            if(loopNode->body) execute(loopNode->body.get());
            currentVal = env.getVar(loopNode->loopVar).num;
            env.setVar(loopNode->loopVar, RuntimeVal(currentVal + stepVal.num));
        }
        return RuntimeVal();
    }

    throw runtime_error("Interpreter Error: Unrecognized AST Node");
}
