#include "Lexer.hpp"
#include <cctype>
#include <stdexcept>
using namespace std;

Lexer::Lexer(string source){
   src=move(source);
   pos=0 ;
   line=1 ;
}

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;
    while(pos < src.length()){
        skipWhitespaceAndComments();
        if (pos >= src.length()) break;

        char c = peek();

        if(isalpha(c) || c == '_'){
            tokens.push_back(readIdentifierOrKeyword());
            continue;
        }
        if(isdigit(c)){
            tokens.push_back(readNumber());
            continue;
        }
        if(c == '"'){
            tokens.push_back(readString());
            continue;
        }

        switch(c){
            case '=':
                advance();
                if (peek() == '=') {
                    advance();
                    tokens.push_back({ TokenType::EQUAL, "==", line });
                } else {
                    tokens.push_back({ TokenType::ASSIGN, "=", line });
                }
                break;
            case '>':
                advance();
                tokens.push_back({ TokenType::GT, ">", line });
                break;
            case '<':
                advance();
                tokens.push_back({ TokenType::LT, "<", line });
                break;
            case '(':
                advance();
                tokens.push_back({ TokenType::LPAREN, "(", line });
                break;
            case ')':
                advance();
                tokens.push_back({ TokenType::RPAREN, ")", line });
                break;
            case '[':
                advance();
                tokens.push_back({ TokenType::LBRACKET, "[", line });
                break;
            case ']':
                advance();
                tokens.push_back({ TokenType::RBRACKET, "]", line });
                break;
            case ',':
                advance();
                tokens.push_back({ TokenType::COMMA, ",", line });
                break;
            case '+':
                advance();
                tokens.push_back({ TokenType::ADD, "+", line });
                break;
            case '-':
                advance();
                tokens.push_back({ TokenType::MINUS, "-", line });
                break;
            default:
                throw runtime_error("Lexical Error: Unknown symbol '"+string(1, c)+"'at line"+to_string(line));
        }
    }

    tokens.push_back({ TokenType::EOF_MARKER, "EOF", line });
    return tokens;
}

void Lexer::skipWhitespaceAndComments() {
    while (pos < src.length()) {
        char c = peek();
        if(isspace(c)) advance();  
        else if (c == '#'){ 
            while (peek()!='\n' && peek()!='\0'){
                pos++; 
            }
        } 
        else{
            break;
        }
    }
}
 
char Lexer::peek()const {
    if (pos>=src.length()) return '\0';
    return src[pos];
}

char Lexer::advance() {
    if(pos >= src.length()) return '\0';
    char c = src[pos++];
    if (c=='\n') line++;
    return c;
}




Token Lexer::readIdentifierOrKeyword() {
    string result;
    while (isalnum(peek()) || peek() == '_') {
        result+=advance();
    }

    if (result == "start") return { TokenType::START, result, line };
    if (result == "end") return { TokenType::END, result, line };
    if (result == "loop") return { TokenType::LOOP, result, line };
    if (result == "check") return { TokenType::CHECK, result, line };
    if (result == "else") {
        size_t savedPos = pos;
        int savedLine = line;
        while (pos<src.length() && isspace(src[pos])) {
            if (src[pos] == '\n') line++;
            pos++;
        }
        if (src.substr(pos,5)=="check") {
            pos += 5;
            return { TokenType::ELSE_CHECK, "else check", line };
        } 
        pos = savedPos;
        line = savedLine;
        return { TokenType::ELSE, result, line };
    }
    if (result == "and") return { TokenType::AND, result, line };
    if (result == "or") return { TokenType::OR, result, line };
    if (result == "in") return { TokenType::IN, result, line };
    if (result == "out") return { TokenType::OUT, result, line };

    return { TokenType::IDENTIFIER, result, line };
}




Token Lexer::readNumber() {
    string result;
    while (isdigit(peek())) {
        result += advance();
    }
    return { TokenType::NUMBER, result, line };
}





Token Lexer::readString() {
    advance();
    string result;
    while(peek()!='"' && peek()!='\0') {
        result += advance();
    }
    if (peek() == '\0') {
        throw std::runtime_error("Lexical Error: Unterminated string literal at line " + std::to_string(line));
    }
    advance(); 
    return { TokenType::STRING, result, line };
}

