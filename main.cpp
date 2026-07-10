#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "Lexer.cpp"
#include "Parser.cpp"
using namespace std;

int main(){
    ifstream file("Source_Code.TL") ;
    stringstream buffer;
    buffer << file.rdbuf();
    string code = buffer.str();
    
    try{
        Lexer lexer(code);
        vector<Token>tokens = lexer.tokenize();

        for (const auto& token : tokens){
            cout<<"Line "<< token.line<<" | Type ID: "<<static_cast<int>(token.type)<< " | Read Text Value: [" <<token.value<< "]"<<endl;
        }

        Parser parser(tokens);
        unique_ptr<BlockASTNode> programAST = parser.parseProgram();

        cout << "--- Parse Success! Tree built successfully in memory with no structural compilation anomalies. ---" << endl ;
    }catch(const exception& e){
        cerr<<e.what()<<endl;
    }

    return 0;
}
