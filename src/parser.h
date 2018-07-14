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
//Parses statment list and returns code
std::vector<std::string> statList(symtable::table_tree* table);
std::vector<std::string> statListPrime(symtable::table_tree* table);
std::vector<std::string> statement(symtable::table_tree* table);
std::vector<std::string> decStat(symtable::table_tree* table);
std::vector<std::string> decStatPrime(symtable::table_tree* table);
std::vector<std::string> assignStat(symtable::table_tree* table);
//parses expressions and terms of operators of independant precedence |&==
std::vector<std::string> expression(symtable::table_tree* table, grammar_type op);
std::vector<std::string> expressionPrime(symtable::table_tree* table, grammar_type op);
//parses terms. if mulDiv is false parses + and -, if true it parses * and /
std::vector<std::string> arithTerm(symtable::table_tree* table, bool mulDiv);
std::vector<std::string> arithTermPrime(symtable::table_tree* table, bool mulDiv);
//Parses values of an expression: vars, literals, calls, (expression)
//Other parser functions:
void advance();
void error(std::string message);
std::vector<data_type> getTypesFromVarDecs(std::vector<var_dec> decs);
bool isTypeName();
}


#endif
