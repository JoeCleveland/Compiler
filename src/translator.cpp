#include "translator.h"
#include <iostream>

int translator::tempVal = 0;
int translator::jumpVal = 0;
std::vector<translator::instruction> translator::intermediateCode;

std::string translator::getTempLabel(){
    return "%" + std::to_string(translator::tempVal++);
}

std::string translator::getJumpLabel(){
    return "_L" + std::to_string(translator::jumpVal++);
}

translator::instruction translator::expressionLine(std::string in1, grammar_type op, std::string in2){
    instruction line;
    line.type = translator::expression;
    line.args.push_back(getTempLabel());
    line.args.push_back(in1);
    line.args.push_back(opToString(op));
    line.args.push_back(in2);
    return line;
}

translator::instruction translator::assignLine(std::string dest, std::string src){
    instruction line;
    line.type = translator::assign;
    line.args.push_back(dest);
    line.args.push_back(src);
    return line;
}

translator::instruction translator::variableLine(symtable::table_tree* tree, bool isParam){
    std::vector<var_dec> vars = tree->getEntries(isParam);
    instruction line;
    if(isParam)
        line.type = translator::params;
    else
        line.type = translator::vars;
    for(var_dec vd : vars){
        line.args.push_back("4");//var size
        line.args.push_back(vd.id);
    }
    return line;
}

translator::instruction translator::callArgLine(std::string id){
    instruction line;
    line.type = translator::callarg;
    line.args.push_back(id);
    return line;
}

translator::instruction translator::callLine(std::string id){
    instruction line;
    line.type = translator::call;
    line.args.push_back(getTempLabel());
    line.args.push_back(id);
    return line;
}

translator::instruction translator::retLine(std::string value){
    return instruction(translator::ret, {value});
}

translator::instruction translator::condJumpLine(std::string result, std::string label){
    instruction line;
    line.type = translator::condjump;
    line.args.push_back(result);
    line.args.push_back(label);
    return line;
} 

void translator::printInstruction(instruction i){
    std::string name;
    switch(i.type){
        case translator::assign: name = "asgn";  break;
        case translator::expression: name = "expr";  break;
        case translator::params: name = "parm";  break;
        case translator::vars: name = "vars";  break;
        case translator::call: name = "call";  break;
        case translator::callarg: name = "clrg";  break;
        case translator::ret: name = "retn";  break;
        case translator::condjump: name = "cjmp";  break;
        case translator::jump: name = "jump";  break;
    }
    std::cout << name << "\t";
    for(std::string s : i.args)
        std::cout << s << " "; 
    std::cout << std::endl;
}


