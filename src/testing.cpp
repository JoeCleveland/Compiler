#include <iostream>
#include <vector>
#include "lexical.h"
#include "parser.h"

int main(){
    std::string program = std::string("fn int, int add (int x, float y)");
    std::vector<token> tokens = lexAnalysis(program);
    parser::tokens = &tokens;
    parser::lookahead = &tokens.at(0);
    parser::defList();
    return 0;
}   
