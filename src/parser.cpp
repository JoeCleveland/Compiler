#include "parser.h"
#include "translator.h"
#include <iostream>

#define LOOK lookahead->type 

token* parser::lookahead;
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
    //parse statements
    std::vector<translator::instruction> code = statList(&fnTable);
    //function:
    code.insert(code.begin(), translator::variableLine(&fnTable, false));//vars
    code.insert(code.begin(), translator::variableLine(&fnTable, true));//params
    //function label:
    code.insert(code.begin(), translator::instruction(translator::label, {fnId}));
    code.insert(code.end(), translator::instruction(translator::ret, {}));
    translator::intermediateCode = catVectors(translator::intermediateCode, code);
    fnTable.printTree();    
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
        dec.isParam = true;
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
    return var_dec(typ, id, false);
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

std::vector<translator::instruction> parser::statList(symtable::table_tree* table){
    std::cout << "STAT_LIST" << std::endl;
    std::vector<translator::instruction> code;
    if(LOOK == O_BRACKET){
        advance(); //past {
        code = statement(table); 
        std::vector<translator::instruction> listCode = statListPrime(table);
        code.insert(code.end(), listCode.begin(), listCode.end());//concat vectors
    }
   return code;
} 

std::vector<translator::instruction> parser::statListPrime(symtable::table_tree* table){
    std::cout << "STAT_LIST_PRIME" << std::endl;
    std::vector<translator::instruction> code;
    if(LOOK == C_BRACKET)
        advance();
    else{
        code = statement(table); 
        std::vector<translator::instruction> listCode = statListPrime(table);
        code.insert(code.end(), listCode.begin(), listCode.end());//concat vectors
    }
    return code;
}


std::vector<translator::instruction> parser::statement(symtable::table_tree* table){
    std::cout << "STATMENT" << std::endl;
    std::cout << lookahead->lexeme << std::endl;
    std::vector<translator::instruction> code;
    if(isTypeName()) //if isTypeName then parse a declaration
        code = decStat(table);
    else if(LOOK == ID) 
        code = assignStat(table);
    return code;
}

std::vector<translator::instruction> parser::decStat(symtable::table_tree* table){
    std::cout << "DEC_STAT" << std::endl;
    std::string dest = table->addEntry(varDec()).id;
    return decStatPrime(table, dest);
}

std::vector<translator::instruction> parser::decStatPrime(symtable::table_tree* table, std::string dest){
    std::cout << "DEC_STAT_PRIME" << std::endl;
    if(LOOK == SEMI){
        advance();
        return std::vector<translator::instruction>();
    }
    else if(LOOK == EQ){
        advance();//=
        exp_ret exp = expression(table, OR);
        //call translator, set var to result of expression
        translator::instruction asgnLine = translator::assignLine(dest, exp.result);
        std::vector<translator::instruction> code = catVectors(exp.code, {asgnLine});
        if(LOOK == SEMI)
            advance();//;
        else
            error("Semicolon expected");
        return code;
    }
    else{
        error("Semicolon or assignment expected");
        return std::vector<translator::instruction>();
    }
}
 
std::vector<translator::instruction> parser::assignStat(symtable::table_tree* table){
    //TODO all of this
}

parser::exp_ret parser::expression(symtable::table_tree* table, grammar_type op){
    grammar_type next_call;
    switch(op){
        case OR:   next_call = AND; break;
        case AND:  next_call = EQEQ; break;
        case EQEQ: next_call = PLUS; break;//PLUSis both + and -
        case SUB: 
        case PLUS: next_call = MULT; break;//MULT is both * and /
        case DIV:  
        case MULT: next_call = ID; break;//ID is for all value terms
    }
    exp_ret leftSide;
    if(next_call == ID)
        leftSide = valueTerm(table);
    else
        leftSide = expression(table, next_call);

    while(LOOK == op || (op == MULT && LOOK == DIV) || (op == PLUS && LOOK == SUB)){
        grammar_type instOp = LOOK;
        advance();//past operator
        exp_ret rightSide = expression(table, next_call);
        translator::instruction thisLine = translator::expressionLine(leftSide.result, 
                instOp, rightSide.result);
        rightSide.code.push_back(thisLine);
        leftSide.code = catVectors(leftSide.code, rightSide.code);
        leftSide.result = thisLine.args[0];//Result register
    }
    return leftSide;
}

parser::exp_ret parser::valueTerm(symtable::table_tree* table){
    std::cout << "VALUE:" << lookahead->lexeme << std::endl;
    exp_ret valRet;
    if(LOOK == ID)
        valRet.result = lookahead->lexeme;
    else if (LOOK == INT_LIT)
        valRet.result = "$" + lookahead->lexeme;
    advance();
    return valRet;
}
