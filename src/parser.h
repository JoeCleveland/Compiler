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
//Parses param list and returns var_decs in list
std::vector<var_dec> paramList();
std::vector<var_dec> paramListPrime();
//Returns a var_dec 
var_dec varDec();
//returns type name, or reports error
data_type typeName();

//Other parser functions:
void advance();
void error(std::string message);
std::vector<data_type> getTypesFromVarDecs(std::vector<var_dec> decs);
}


#endif
