#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "consts.h"
#include "symtable.h"
#include <vector>
#include <string>

namespace translator {
    enum inst_type { assign, expression, params, vars, label, call, callarg, ret, condjump, jump};
    typedef struct instruction {
        inst_type type;
        std::vector<std::string> args;
        instruction () {}
        instruction (inst_type t, std::vector<std::string> a) : type(t), args(a) {}
    } instruction;
    extern int tempVal;
    extern int jumpVal;
    extern std::vector<instruction> intermediateCode;
    std::string getTempLabel();
    std::string getJumpLabel();
    instruction expressionLine(std::string in1, grammar_type op, std::string in2);
    instruction assignLine(std::string dest, std::string src);
    instruction variableLine(symtable::table_tree* tree, bool param);
    instruction callArgLine(std::string id);
    instruction callLine(std::string id);
    instruction retLine(std::string value);
    instruction condJumpLine(std::string result, std::string label);
    
    void printInstruction(instruction i);
}
#endif
