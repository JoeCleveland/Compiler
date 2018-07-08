#include "parser.h"
#include <iostream>

#define LOOK lookahead->type 

token* parser::lookahead;
std::vector<token>* parser::tokens;
symtable::global_table parser::symbolTable;

void parser::advance(){
    lookahead++;
}

void parser::error(std::string message){
    std::cout << message << std::endl;
}

void parser::defList(){ 
    std::cout << "DEF_LIST" << std::endl;
    if(LOOK == FN){
        funcDef();
        defList();
    }
}

void parser::funcDef(){
    std::cout << "FUNC_DEF" << std::endl;
    advance();//past fn
    //parse ret list
    std::vector<data_type> fnRetList = retList();
    //parse id
    std::string fnId = lookahead->lexeme;
    std::cout << fnId << std::endl;
    //Add entry to global table
    symtable::func_entry thisFunc;
    thisFunc.lexeme = fnId;
    thisFunc.retTypes = fnRetList;
    advance();//past id
    //parse param list
    //paramList();
    //TODO stat list
}

std::vector<data_type> parser::retList(){
    std::cout << "RET_LIST" << std::endl;
    std::vector<data_type> list;
    if(LOOK == O_PAREN){
        advance();
    	data_type typ = typeName();
        list = retListPrime();	
	    list.insert(list.begin(), typ);
    }
    return list;
}

std::vector<data_type> parser::retListPrime() {
    std::vector<data_type> list;
    if(LOOK == COMMA){
        advance();
        data_type typ = typeName();
        list = retListPrime();
        list.insert(list.begin(), typ);
    }    
    else if(LOOK == C_PAREN)
       advance();
    else
       error("Expected ')' at end of return list");
    return list;
} 
    
std::vector<var_def> parser::paramList(){
    std::cout << "PARAM_LIST" << std::endl;
    std::vector<var_def> list;
    if(LOOK == O_PAREN){
        advance();
    	var_def def = varDef();
        list = paramListPrime();	
	    list.insert(list.begin(), def);
    }
    return list;
}

std::vector<var_def> parser::paramListPrime(){
    std::vector<var_def> list;
    if(LOOK == COMMA){
        advance();
    	var_def def = varDef();
        list = paramListPrime();	
        list.insert(list.begin(), def);
    }
    else if(LOOK == C_PAREN)
       advance();
    else
       error("Expected ')' at end of parameter list");
    return list;
}

var_def parser::varDef(){
    data_type typ = typeName();
    std::string id = lookahead->lexeme;
    advance();// past id 
    return var_def(typ, id);
}

data_type parser::typeName(){
    if(LOOK == INT){
        advance();
        std::cout << "int" << std::endl;
        return data_type(TYPE_PRIM_INT, -1);
    }
    else if(LOOK == FLOAT){
        advance();
        std::cout << "float" << std::endl;
        return data_type(TYPE_PRIM_FLOAT, -1);
    }
}
