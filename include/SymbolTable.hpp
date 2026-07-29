#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP
using namespace std;  

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

// Represents a value in memory during execution
struct RuntimeVal {
    enum Type { NUMBER, STRING, ARRAY, NONE } type = NONE;
    int num = 0;
 string str = "";
 vector<RuntimeVal> array;

    RuntimeVal() {}
    RuntimeVal(int n) : type(NUMBER), num(n) {}
    RuntimeVal (string s) : type(STRING), str(s) {}
    RuntimeVal (vector<RuntimeVal> a) : type(ARRAY), array(a) {}

 string toString() const {
        if (type == NUMBER) return to_string(num);
        if (type == STRING) return str;
        if (type == ARRAY) {
         string res = "[";
            for (size_t i = 0; i < array.size(); ++i) {
                res += array[i].toString();
                if (i < array.size() - 1) res += ", ";
            }
            res += "]";
            return res;
        }
        return "null";
    }

    bool isTruthy() const {
        if (type == NUMBER) return num != 0;
        if (type == STRING) return str != "" && str != "0" && str != "false";
        return false;
    }
};

class SymbolTable {
private:
 unordered_map <string, RuntimeVal> table;

public:
    void setVar(const string& name, const RuntimeVal& val) {
        table[name] = val;
    }

    RuntimeVal getVar(const string& name){
        if(table.find(name)==table.end()){
            throw runtime_error("Runtime Error: Undefined variable '"+name+"'");
        }
        return table[name];
    }

    void setIndex(const string& name, int index, const RuntimeVal& val){
        if(table.find(name) == table.end()||table[name].type != RuntimeVal::ARRAY) {
            throw runtime_error("Runtime Error: '" + name + "' is not an array.");
        }
        if(index<0 || index>=static_cast<int>(table[name].array.size())){
            throw runtime_error("Runtime Error: Array index out of bounds.");
        }
        table[name].array[index] = val;
    }

    RuntimeVal getIndex(const string& name, int index){
        if(table.find(name)==table.end()||table[name].type!=RuntimeVal::ARRAY){
            throw runtime_error("Runtime Error: '" + name + "' is not an array.");
        }
        if(index<0 || index>=static_cast<int>(table[name].array.size())){
            throw runtime_error("Runtime Error: Array index out of bounds.");
        }
        return table[name].array[index];
    }
};

#endif 