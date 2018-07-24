#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "consts.h"
#include <vector>
#include <string>

namespace translator {
    enum inst_type { assign, expression, alloc, label, call, jmp, jg, jge, jl, jle, je, jne, ret};
    typedef struct instruction {
        inst_type type;
        std::vector<std::string> args;
        instruction () {}
        instruction (inst_type t, std::vector<std::string> a) : type(t), args(a) {}
    } instruction;
    extern int tempVal;
    extern std::vector<instruction> intermediateCode;
    std::string getTempLabel();
    instruction expressionLine(std::string in1, grammar_type op, std::string in2);
    instruction assignLine(std::string dest, std::string src);
    
    void printInstruction(instruction i);
}
#endif
