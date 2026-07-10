#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>
#include <memory>
#include "Lexer.hpp"
#include "AST.hpp"

class Parser {
private:
    vector<Token> tokens;
    size_t current = 0;
    Token peek()const;
    Token previous()const;
    bool isAtEnd()const;
    Token advance();
    bool checkType(TokenType type)const;
    bool match(vector<TokenType> types);
    Token consume(TokenType type, string errorMessage);

    unique_ptr<ASTNode> parseStatement();
    unique_ptr<ASTNode> parseAssignmentOrExpr();
    
    // Expression Parsing Hierarchy
    unique_ptr<ASTNode> parseExpression();
    unique_ptr<ASTNode> parseLogicalOr();
    unique_ptr<ASTNode> parseLogicalAnd();
    unique_ptr<ASTNode> parseEquality();
    unique_ptr<ASTNode> parseTerm(); 
    unique_ptr<ASTNode> parsePrimary();

public:
    Parser(vector<Token> srcTokens);
    unique_ptr<BlockASTNode> parseProgram();
};

#endif // PARSER_HPP