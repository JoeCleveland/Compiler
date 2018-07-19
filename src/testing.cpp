#include <iostream>
#include <vector>
#include "lexical.h"
#include "parser.h"

int main(){
    std::string program = std::string("fn (int) add (int x, int y) { int z = x + y * a; int b = z | b;}");
    std::vector<token> tokens = lexAnalysis(program);
    parser::tokens = &tokens;
    parser::lookahead = &tokens.at(0);
    parser::defList();
    return 0;
}   
