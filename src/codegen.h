#ifndef CODEGEN_H
#define CODEGEN_H

#include "translator.h"
#include <string>
#include <vector>
#include <map>

namespace codegen {
    extern std::vector<std::string> buffer;
    extern std::string argOrder[];
    extern std::vector<std::string> availableRegisters;
    extern std::map<std::string, int> offsets;
    extern int lastOffset;
    extern std::map<std::string, std::string> tempToReg;

    void writeLine(translator::instruction inst);
    void outputToFile(std::string fileName);
}
#endif
