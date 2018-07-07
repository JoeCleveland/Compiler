#include "symtable.h"

symtable::table_tree::table_tree(symtable::table_tree* parent){
    this->parent = parent;
}

void symtable::table_tree::addEntry(symtable::table_entry e){
    table.push_back(e);
}

void symtable::table_tree::addChild(table_tree* t){
    children.push_back(t);
}

symtable::data_type symtable::table_tree::getType(std::string lexeme){
    for(int i = 0; i < table.size(); i++)
        if(!table[i].lexeme.compare(lexeme))
            return table[i].type;
    throw std::invalid_argument("Lexeme not found in scope");
}

