#include "symtable.h"
#include "consts.h"
#include <iostream>
#include <stdexcept>

using namespace symtable;

/** table tree  **/
table_tree::table_tree(table_tree* parent){
    this->parent = parent;
}

table_tree::table_tree(std::vector<var_dec> t):table(t){}

var_dec table_tree::addEntry(var_dec d){
    table.push_back(d);
    return d;
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

void table_tree::printTree(){
    std::cout << "----" << std::endl;
    for(var_dec v : table)
        std::cout << v.id << v.isParam << std::endl;
    for(table_tree* t : children)
        t->printTree();
}

std::vector<var_dec> table_tree::getEntries(bool param){
    std::vector<var_dec> output;
    for(var_dec vd : table)
        if(param == vd.isParam)
            output.push_back(vd);
    for(table_tree* t : children)
        output = catVectors(output, t->getEntries(param));
    return output;
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
