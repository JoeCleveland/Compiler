#include "translator.h"

int translator::tempVal = 0;

std::string translator::getTempLabel(){
    return "t" + std::to_string(translator::tempVal++);
}

parser::exp_ret translator::expressionLine(std::string in1, grammar_type op, std::string in2){
    std::string dest = translator::getTempLabel();
    std::string code = dest + " = " + in1 + " " + opToString(op) + " " + in2;
    return parser::exp_ret({code}, dest, false);
}

std::string translator::assignLine(std::string dest, std::string src){
    return dest + " = " + src;
}
