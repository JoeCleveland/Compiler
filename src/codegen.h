#ifndef CODEGEN_H
#define CODEGEN_H

#include <string>
#include <vector>

namespace codegen {
    extern std::vector<std::string> buffer;
    extern std::vector<std::string> availableRegisters;

    void writeLine(translator::instruction inst);
    void outputToFile(std::string fileName);
}
#endif
