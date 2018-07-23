#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "consts.h"
#include "parser.h"
#include <vector>
#include <string>

namespace translator {
    enum inst_type { assign };
    typedef struct instruction {
        std::string dest;
        std::string op1;
        std::string op2;
        inst_type type;
    } inst_type;
    extern int tempVal;
    std::string getTempLabel();
    parser::exp_ret expressionLine(std::string in1, grammar_type op, std::string in2);
    std::string assignLine(std::string dest, std::string src);
}
#endif
