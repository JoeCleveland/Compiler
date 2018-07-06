#include <iostream>
#include <vector>
#include "lexical.h"
#include "parser.h"


int main(){
    std::string program = std::string("fn int add(int 1x, int 45)");
    std::vector<token> tokens = lexAnalysis(program);
    parser::tokens = &tokens;
    return 0;
}   
