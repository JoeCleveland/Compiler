#include "symtable.h"
#include "consts.h"
#include <stdexcept>

using namespace symtable;

/** table tree  **/
table_tree::table_tree(table_tree* parent){
    this->parent = parent;
}

table_tree::table_tree(std::vector<var_dec> t):table(t){}

void table_tree::addEntry(var_dec d){
    table.push_back(d);
}

void table_tree::addChild(table_tree* t){
    children.push_back(t);
}

data_type table_tree::getType(std::string lexeme){
    for(int i = 0; i < table.size(); i++)
        if(!table[i].id.compare(lexeme))
            return table[i].type;
    throw std::invalid_argument("Lexeme not found in scope");
}

/** global table **/
void global_table::addEntry(global_entry e){
    table.push_back(e);
}

global_entry global_table::getEntry(std::string lexeme){
    for(int i = 0; i < table.size(); i++)
        if(!table[i].lexeme.compare(lexeme))
            return table[i];
    throw std::invalid_argument("Lexeme not found in program");
}
