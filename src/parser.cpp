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

std::vector<data_type> parser::getTypesFromVarDecs(std::vector<var_dec> decs){
    std::vector<data_type> output;
    for(int i = 0; i < decs.size(); i++)
        output.push_back(decs[i].type);
    return output;
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
    advance();//past id
    //parse param list
    std::vector<var_dec> fnParamList = paramList();
    //TODO stat list
    symtable::table_tree root(fnParamList); 
    //Add entry to global table
    symtable::func_entry thisFunc;
    thisFunc.lexeme = fnId;
    thisFunc.retTypes = fnRetList;
    thisFunc.argTypes = getTypesFromVarDecs(fnParamList); 
    symbolTable.addEntry(thisFunc);
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
    
std::vector<var_dec> parser::paramList(){
    std::cout << "PARAM_LIST" << std::endl;
    std::vector<var_dec> list;
    if(LOOK == O_PAREN){
        advance();
    	var_dec dec = varDec();
        list = paramListPrime();	
	    list.insert(list.begin(), dec);
    }
    return list;
}

std::vector<var_dec> parser::paramListPrime(){
    std::vector<var_dec> list;
    if(LOOK == COMMA){
        advance();
    	var_dec dec = varDec();
        list = paramListPrime();	
        list.insert(list.begin(), dec);
    }
    else if(LOOK == C_PAREN)
       advance();
    else
       error("Expected ')' at end of parameter list");
    return list;
}

var_dec parser::varDec(){
    data_type typ = typeName();
    std::string id = lookahead->lexeme;
    std::cout << id << std::endl;
    advance();// past id 
    return var_dec(typ, id);
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
