#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#using namespace std ;

enum class TokenType {
    START, END, LOOP, CHECK, ELSE_CHECK, ELSE,
    IN, OUT, IDENTIFIER, NUMBER, STRING,
    ASSIGN, EQUAL, GT, LT, AND, OR,
    LPAREN, RPAREN, LBRACKET, RBRACKET, COMMA, EOF_MARKER
};

struct Token {
    TokenType type;
    string value;
    int line;
};


#endif