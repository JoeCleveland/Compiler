#include <iostream>
#include <vector>
#include "lexical.h"
#include "parser.h"

int main(){
    std::string program = std::string("fn (int, int) add (int x, int y) { int a = x + y * z;}");
    std::vector<token> tokens = lexAnalysis(program);
    parser::lookahead = &tokens.at(0);
    parser::defList();
    for(translator::instruction i : translator::intermediateCode)
        translator::printInstruction(i);
    return 0;
}   
