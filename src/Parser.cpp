#include "Parser.hpp"
#include <stdexcept>

Parser::Parser(vector<Token> srcTokens) : tokens(move(srcTokens)), current(0){}

Token Parser::peek() const{ return tokens[current];}
Token Parser::previous() const{ return tokens[current - 1];}
bool Parser::isAtEnd() const{ return peek().type == TokenType::EOF_MARKER;}

Token Parser::advance(){
    if(!isAtEnd()) current++;
    return previous();
}

bool Parser::checkType(TokenType type) const{
    if(isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(vector<TokenType> types){
    for(TokenType type : types){
        if (checkType(type)){
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, string errorMessage){
    if (checkType(type)) return advance();
    throw runtime_error("Parser Error at line " + to_string(peek().line) + ": " + errorMessage);
}

// STATEMENT PARSING
unique_ptr<BlockASTNode> Parser::parseProgram(){
    auto programBlock = make_unique<BlockASTNode>();
    consume(TokenType::START, "Expected 'start' keyword at beginning of file.");

    while (!checkType(TokenType::END) && !isAtEnd()){
        programBlock->statements.push_back(parseStatement());
    }
    consume(TokenType::END, "Expected 'end' keyword at the end of the program.");
    return programBlock;
}

unique_ptr<ASTNode> Parser::parseStatement() {
    // Parse OUT out( ... )
    if (match({TokenType::OUT})){
        consume(TokenType::LPAREN, "Expected '(' after 'out'");
        auto expr = parseExpression();
        consume(TokenType::RPAREN, "Expected ')' after out parameter list");
        return make_unique<OutASTNode>(move(expr));
    }

    // Parse IN in(choice)
    if (match({TokenType::IN})){
        consume(TokenType::LPAREN, "Expected '(' after 'in'");
        Token varToken = consume(TokenType::IDENTIFIER, "Expected a variable name inside 'in()'");
        consume(TokenType::RPAREN, "Expected ')' after variable name");
        return make_unique<InputASTNode>(varToken.value);
    }

    // Parse LOOP loop(var, start, end, step)
    if (match({TokenType::LOOP})){
        consume(TokenType::LPAREN, "Expected '(' after 'loop'");
        Token varToken = consume(TokenType::IDENTIFIER, "Expected a loop variable");
        consume(TokenType::COMMA, "Expected ',' after loop variable");
        auto startExpr = parseExpression();
        consume(TokenType::COMMA, "Expected ',' after start value");
        auto endExpr = parseExpression();
        consume(TokenType::COMMA, "Expected ',' after end value");
        auto stepExpr = parseExpression();
        consume(TokenType::RPAREN, "Expected ')' after loop parameters");

        auto body = make_unique<BlockASTNode>();
        while (!checkType(TokenType::END) && !isAtEnd()){
            body->statements.push_back(parseStatement());
        }
        consume(TokenType::END, "Expected 'end' to finish loop block.");

        return make_unique<LoopASTNode>(varToken.value, move(startExpr), move(endExpr), move(stepExpr), move(body));
    }

    // Parse CHECK check ( ... )
    if (match({TokenType::CHECK})){
        consume(TokenType::LPAREN, "Expected '(' before check condition");
        auto condition = parseExpression();
        consume(TokenType::RPAREN, "Expected ')' after condition");

        auto body = make_unique<BlockASTNode>();
        while (!checkType(TokenType::END) && !checkType(TokenType::ELSE) && !isAtEnd()) {
            body->statements.push_back(parseStatement());
        }

        auto checkNode = make_unique<CheckASTNode>(move(condition), move(body));

        if (match({TokenType::ELSE})){
            auto elseBody = make_unique<BlockASTNode>();
            while (!checkType(TokenType::END) && !isAtEnd()){
                elseBody->statements.push_back(parseStatement());
            }
            checkNode->elseBody = move(elseBody);
        }

        consume(TokenType::END, "Expected 'end' keyword to finish check block.");
        return checkNode;
    }

    return parseAssignmentOrExpr();
}

unique_ptr<ASTNode> Parser::parseAssignmentOrExpr(){
    auto expr = parseExpression();
    if (checkType(TokenType::ASSIGN)){
        advance(); 
        auto value = parseExpression();

        if (auto idNode = dynamic_cast<IdentifierASTNode*>(expr.get())){
            return make_unique<AssignmentASTNode>(idNode->name, move(value));
        }else if(auto indexNode = dynamic_cast<IndexASTNode*>(expr.get())){
            return make_unique<ArrayAssignmentASTNode>(indexNode->arrayName, move(indexNode->index), move(value));
        }
        throw runtime_error("Parser Error: Invalid assignment target.");
    }
    return expr;
}

// EXPRESSION PARSING (Order of Operations)
unique_ptr<ASTNode> Parser::parseExpression(){
    return parseLogicalOr();
}

unique_ptr<ASTNode> Parser::parseLogicalOr(){
    auto expr = parseLogicalAnd();
    while (match({TokenType::OR})) {
        string op = previous().value;
        auto right = parseLogicalAnd();
        expr = make_unique<BinaryExprASTNode>(op, move(expr), move(right));
    }
    return expr;
}

unique_ptr<ASTNode> Parser::parseLogicalAnd(){
    auto expr = parseEquality();
    while (match({TokenType::AND})) {
        string op = previous().value;
        auto right = parseEquality();
        expr = make_unique<BinaryExprASTNode>(op, move(expr), move(right));
    }
    return expr;
}

unique_ptr<ASTNode> Parser::parseEquality(){
    auto expr = parseTerm();
    while (match({TokenType::EQUAL, TokenType::GT, TokenType::LT})) {
        string op = previous().value;
        auto right = parseTerm();
        expr = make_unique<BinaryExprASTNode>(op, move(expr), move(right));
    }
    return expr;
}

unique_ptr<ASTNode> Parser::parseTerm(){
    auto expr = parsePrimary();
    // Uses ADD and MINUS as per your token requirements
    while (match({TokenType::ADD, TokenType::MINUS})) {
        string op = previous().value;
        auto right = parsePrimary();
        expr = make_unique<BinaryExprASTNode>(op, move(expr), move(right));
    }
    return expr;
}

unique_ptr<ASTNode> Parser::parsePrimary(){
    if (match({TokenType::NUMBER})) {
        return make_unique<NumberASTNode>(stoi(previous().value));
    }
    if (match({TokenType::STRING})) {
        return make_unique<StringASTNode>(previous().value);
    }
    if (match({TokenType::IDENTIFIER})) {
        string varName = previous().value;
        if (match({TokenType::LBRACKET})) {
            auto indexExpr = parseExpression();
            consume(TokenType::RBRACKET, "Expected ']' after array index.");
            return make_unique<IndexASTNode>(varName, move(indexExpr));
        }
        return make_unique<IdentifierASTNode>(varName);
    }
    
    if (match({TokenType::LBRACKET})){
        vector<unique_ptr<ASTNode>> elements;
        if (!checkType(TokenType::RBRACKET)) {
            do {
                elements.push_back(parseExpression());
            } while (match({TokenType::COMMA}));
        }
        consume(TokenType::RBRACKET, "Expected ']' at the end of array.");
        return make_unique<ArrayASTNode>(move(elements));
    }
    
    if (match({TokenType::LPAREN})){
        auto expr = parseExpression();
        consume(TokenType::RPAREN, "Expected ')' after expression.");
        return expr;
    }

    throw runtime_error("Parser Error: Unexpected token '" + peek().value + "' at line " + to_string(peek().line));
}