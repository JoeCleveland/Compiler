#include <iostream>
#include <vector>
#include "lexical.h"
#include "parser.h"
#include "codegen.h"

int main(){
    std::string program = std::string("fn (int, int) sum (int x) {" 
            "int a = x * x + x + x;}");
    std::vector<token> tokens = lexAnalysis(program);
    parser::lookahead = &tokens.at(0);
    parser::defList();
    for(translator::instruction i : translator::intermediateCode){
        translator::printInstruction(i);
        codegen::writeLine(i);
    }
    codegen::outputToFile("code.s");
    return 0;
}   
