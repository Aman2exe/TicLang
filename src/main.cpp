#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "AST.hpp"

string readFile(const string& filepath) {
    ifstream file(filepath);
   if (!file.is_open()) {
        throw runtime_error("Error: Could not open file '" + filepath + "'");
    } 
    
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    // Default to your specific file if no command line argument is given
    string filepath = "Source_Code.TL"; 
    
    // If you do provide a file via terminal (e.g., ./ticlang other_script.tl)
    if (argc == 2) {
        filepath = argv[1];
    } else if (argc > 2) {
        cerr << "Usage: ./ticlang [path_to_script.tl]\n";
        return 1;
    }

    try {
        // 1. Read the script file
        string sourceCode = readFile(filepath);

        // 2. Lexical Analysis (Text -> Tokens)
        Lexer lexer(sourceCode);
        vector<Token> tokens = lexer.tokenize();

        // 3. Parsing (Tokens -> AST)
        Parser parser(tokens);
        unique_ptr<BlockASTNode> ast = parser.parseProgram();

        // 4. Execution (AST -> Output)
        Interpreter interpreter;
        interpreter.execute(ast.get());

    } catch (const exception& e) {
        // Catch and print any Lexer, Parser, or Interpreter errors
        cerr << "\n[TicLang Error] " << e.what() << "\n";
        return 1;
    }

    return 0;
}