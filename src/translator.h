#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "consts.h"
#include "parser.h"
#include <vector>
#include <string>

namespace translator {
    extern int tempVal;
    std::string getTempLabel();
    parser::exp_ret expressionLine(std::string in1, grammar_type op, std::string in2);
}
#endif
