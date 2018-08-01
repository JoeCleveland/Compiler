#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lexical.h"
#include "parser.h"
#include "codegen.h"

int main(int argc, char** argv){
    std::ifstream codefile(argv[1]);
    std::stringstream codebuffer;
    codebuffer << codefile.rdbuf();
    std::string program = codebuffer.str(); 
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
