#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "consts.h"
#include <string>
#include <vector>

namespace symtable {
    
    class table_tree {
    public:
        table_tree(table_tree* parent);
        table_tree(std::vector<var_dec> t);
        void addEntry(var_dec d);
        void addChild(table_tree* t);
        data_type getType(std::string lexeme);
        void printTree();
    private:
        table_tree* parent;
        std::vector<table_tree*> children;
        std::vector<var_dec> table;
    };

    typedef struct {
        std::string lexeme; 
    } global_entry;

    typedef struct func_entry : global_entry {
        std::vector<data_type> retTypes;
        std::vector<data_type> argTypes;
    } func_entry;
    
    class global_table {
    public:
        void addEntry(global_entry e);
        global_entry getEntry(std::string lexeme); 
    private:
        std::vector<global_entry> table;
    };
}
#endif
