#ifndef PARSER_H
#define PARSER_H

#include "consts.h"
#include "lexical.h"
#include "symtable.h"
#include "translator.h"
#include <vector>
#include <memory>

namespace parser{
extern token* lookahead;
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
std::vector<translator::instruction> statList(symtable::table_tree* table);
std::vector<translator::instruction> statListPrime(symtable::table_tree* table);
std::vector<translator::instruction> statement(symtable::table_tree* table);
std::vector<translator::instruction> decStat(symtable::table_tree* table);
std::vector<translator::instruction> decStatPrime(symtable::table_tree* table, std::string dest);
std::vector<translator::instruction> assignStat(symtable::table_tree* table);
//parses expressions of all math/logical operators:
typedef struct exp_ret {
    std::vector<translator::instruction> code;
    std::string result;
    bool emptyParse;
    exp_ret(){}
    exp_ret(std::vector<translator::instruction> c, std::string r, bool e)
        :code(c), result(r), emptyParse(e) {}
} exp_ret;
exp_ret expression(symtable::table_tree* table, grammar_type op);
//Parses values of an expression: vars, literals, calls, (expression)
exp_ret valueTerm(symtable::table_tree* table);
//Other parser functions:
void advance();
void error(std::string message);
std::vector<data_type> getTypesFromVarDecs(std::vector<var_dec> decs);
bool isTypeName();

}


#endif
