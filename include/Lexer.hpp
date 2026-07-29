#ifndef LEXER_HPP
#define LEXER_HPP

#include<string>
#include<vector>
using namespace std;

enum class TokenType{
    START, END, LOOP, CHECK, ELSE_CHECK, ELSE,
    IN, OUT, IDENTIFIER, NUMBER, STRING,
    ASSIGN, EQUAL, GT, LT, AND, OR,
    LPAREN, RPAREN, LBRACKET, RBRACKET, COMMA, EOF_MARKER,
    ADD, MINUS
};

struct Token{
    TokenType type;
    string value;
    int line;
};

class Lexer{
private:
    string src;
    size_t pos; 
    int line;
    char peek() const;
    char advance();
    void skipWhitespaceAndComments();
    Token readIdentifierOrKeyword();
    Token readNumber(); 
    Token readString();

public:
    Lexer(string source);
    vector<Token> tokenize();
};

#endif 