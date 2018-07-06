#ifndef PARSER_H
#define PARSER_H

#include "consts.h"
#include "lexical.h"
#include <vector>
#include <memory>

namespace parser{
extern token* lookahead;
extern std::vector<token>* tokens;

void defList();
void funcDef();
//Parses ret list and returns return types in list
std::vector<grammar_type> retList();
std::vector<grammar_type> retListPrime();
//Parses param list and adds to symbol table
void paramList();
void paramListPrime();
//Returns a var_def 
typedef struct var_def{
    grammar_type type;
    std::string id;
} var_def;
var_def varDef();
//returns type name, or reports error
grammar_type typeName();

//Other parser functions:
void advance();
void error(std::string message);
}


#endif
