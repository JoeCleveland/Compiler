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

bool parser::isTypeName(){
    return LOOK == INT || LOOK == FLOAT;
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
    symtable::table_tree root(fnParamList); 
    //Add entry to global table
    symtable::func_entry thisFunc;
    thisFunc.lexeme = fnId;
    thisFunc.retTypes = fnRetList;
    thisFunc.argTypes = getTypesFromVarDecs(fnParamList); 
    symbolTable.addEntry(thisFunc);
    //Create table tree
    symtable::table_tree fnTable(fnParamList);
    statList(&fnTable);
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

std::vector<std::string> parser::statList(symtable::table_tree* table){
    std::cout << "STAT_LIST" << std::endl;
    std::vector<std::string> code;
    if(LOOK == O_BRACKET){
        advance(); //past {
        code = statement(table); 
        std::vector<std::string> listCode = statListPrime(table);
        code.insert(code.end(), listCode.begin(), listCode.end());//concat vectors
    }
   return code;
} 

std::vector<std::string> parser::statListPrime(symtable::table_tree* table){
    std::cout << "STAT_LIST_PRIME" << std::endl;
    std::vector<std::string> code;
    if(LOOK == C_BRACKET)
        advance();
    else{
        code = statement(table); 
        std::vector<std::string> listCode = statListPrime(table);
        code.insert(code.end(), listCode.begin(), listCode.end());//concat vectors
    }
    return code;
}


std::vector<std::string> parser::statement(symtable::table_tree* table){
    std::cout << "STATMENT" << std::endl;
    std::vector<std::string> code;
    if(isTypeName()) //if isTypeName then parse a declaration
        code = decStat(table);
    else if(LOOK == ID) 
        code = assignStat(table);
    return code;
}

std::vector<std::string> parser::decStat(symtable::table_tree* table){
    table->addEntry(varDec());
    return decStatPrime(table);
}

std::vector<std::string> parser::decStatPrime(symtable::table_tree* table){
    std::cout << "DEC_STAT_PRIME" << std::endl;
    if(LOOK == SEMI){
        advance();
        return std::vector<std::string>();
    }
    else if(LOOK == EQ){
        advance();
        std::vector<std::string> code = expression(table, OR).code;
        //TODO call translator, set var to result of expression
        return code;
    }
}
 
std::vector<std::string> parser::assignStat(symtable::table_tree* table){
    //TODO all of this
}

parser::exp_ret parser::expression(symtable::table_tree* table, grammar_type op){
    std::cout << "EXPRESSION: " << op << std::endl;
    grammar_type next_call;
    switch(op){
        case OR:   next_call = AND; break;
        case AND:  next_call = EQEQ; break;
        case EQEQ: next_call = PLUS; break;//PLUSis both + and -
        case PLUS:  next_call = MULT; break;//MULT is both * and /
        case MULT: next_call = ID; break;//ID is for all value terms
    }
    exp_ret leftSide;
    if(next_call == ID)
        leftSide = valueTerm(table);
    else
        leftSide = expression(table, next_call);
    exp_ret rightSide = expressionPrime(table, op, leftSide.result);
    exp_ret combine;
    combine.code = catVectors(leftSide.code, rightSide.code);
    if(rightSide.emptyParse)
        combine.result = leftSide.result;
    else
        combine.result = rightSide.result;
    combine.emptyParse = false;
    return combine;
}

parser::exp_ret parser::expressionPrime(symtable::table_tree* table, grammar_type op, std::string leftResult){
    std::cout << "EXPRESSION_PRIME: " << op << std::endl;
    if(LOOK == op){//parse operator
        advance();//past operator
        exp_ret rightSide = expression(table, op);
        //TODO call translator, write atomic expression
    }
    else{
        std::cout << "empty" << std::endl;
        exp_ret emptyRet;
        emptyRet.emptyParse = true;
        return emptyRet;
    } 
}

parser::exp_ret parser::valueTerm(symtable::table_tree* table){
    std::cout << "VALUE:" << lookahead->lexeme << std::endl;
    exp_ret valRet;
    valRet.result = lookahead->lexeme;
    advance();
    return valRet;
}
