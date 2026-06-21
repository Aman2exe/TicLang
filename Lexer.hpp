#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

enum class TokenType {
    START, END, LOOP, CHECK, ELSE_CHECK, ELSE,
    IN, OUT, IDENTIFIER, NUMBER, STRING,
    ASSIGN, EQUAL, GT, LT, AND, OR,
    LPAREN, RPAREN, LBRACKET, RBRACKET, COMMA, EOF_MARKER
};

struct Token {
    TokenType type;
    std::string value;
    int line;
};

class Lexer {
private:
    std::string src;
    size_t pos;
    int line;

    char peek() const;
    char advance();
    void skipWhitespaceAndComments();
    Token readIdentifierOrKeyword();
    Token readNumber();
    Token readString();

public:
    Lexer(std::string source);
    std::vector<Token> tokenize();
};

#endif // LEXER_HPP