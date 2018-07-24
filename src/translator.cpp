#include "translator.h"
#include <iostream>

int translator::tempVal = 0;
std::vector<translator::instruction> translator::intermediateCode;

std::string translator::getTempLabel(){
    return "t" + std::to_string(translator::tempVal++);
}

translator::instruction translator::expressionLine(std::string in1, grammar_type op, std::string in2){
    instruction line;
    line.type = inst_type::expression;
    line.args.push_back(getTempLabel());
    line.args.push_back(in1);
    line.args.push_back(opToString(op));
    line.args.push_back(in2);
    return line;
}

translator::instruction translator::assignLine(std::string dest, std::string src){
    instruction line;
    line.type = inst_type::assign;
    line.args.push_back(dest);
    line.args.push_back(src);
    return line;
}

void translator::printInstruction(instruction i){
    std::cout << i.type << " ";
    for(std::string s : i.args)
        std::cout << s << " "; 
    std::cout << std::endl;
}
