#ifndef VM_H
#define VM_H

#include "translator.h"

namespace vm {
    void runProgram(std::vector<translator::instruction> code);
}
#endif 
