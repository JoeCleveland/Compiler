#include "parser.h"

token* parser::lookahead;
std::vector<token>* parser::tokens;

void parser::advance(){
    lookahead++;
}

void parser::defList(){ 
    if(parser::lookahead->type == FN){
        parser::funcDef();
        parser::defList();
    }
}

void parser::funcDef(){
    advance();//past fn
    //parse ret list
    std::vector<grammar_type> fnRetList = retList();
    //parse id
    std::string fnId = parser::lookahead->lexeme;
    advance();
    //parse param list
    paramList();
    //TODO symbol table and stat list
}

std::vector<grammar_type> parser::retList(){
    if(parser::lookahead->type == O_PAREN){
        advance();
        
    } 
}

