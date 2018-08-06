#ifndef CODEGEN_H
#define CODEGEN_H

#include "translator.h"
#include <string>
#include <vector>
#include <map>

namespace codegen {
    enum loc_type {reg, stack, dynamic };
    typedef struct location {
        loc_type type;
        std::string code;
        location () {};
        location (loc_type t, std::string c) : type(t), code(c) {};
    } location;
    extern std::vector<std::string> buffer;
    extern std::string argOrder[];
    extern std::vector<std::string> availableRegisters;
    extern std::map<std::string, int> offsets;
    extern bool mainFunc;
    extern int lastOffset;
    extern int argCount;
    extern std::map<std::string, location> tempToLoc;

    void writeLine(translator::instruction inst);
    void writeExpressionLine(translator::instruction inst);

    location getTemp(std::string temp);
    location getLoc(std::string tempOrVar);
    std::string getReg();
    std::string fmtReg(std::string reg, int bytes);
    std::string fmtValue(std::string val); 
    std::string fmtLoc(location loc);
    void outputToFile(std::string fileName);
}
#endif
