#ifndef PARSER_H
#define PARSER_H

#include "consts.h"
#include "lexical.h"
#include "symtable.h"
#include <vector>
#include <memory>

namespace parser{
extern token* lookahead;
extern std::vector<token>* tokens;
extern symtable::global_table symbolTable;

void defList();
void funcDef();
//Parses ret list and returns return types in list
std::vector<data_type> retList();
std::vector<data_type> retListPrime();
//Parses param list and returns var_defs in list
std::vector<var_def> paramList();
std::vector<var_def> paramListPrime();
//Returns a var_def 
var_def varDef();
//returns type name, or reports error
data_type typeName();

//Other parser functions:
void advance();
void error(std::string message);
}


#endif
