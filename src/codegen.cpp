#include <fstream>
#include "codegen.h"

std::vector<std::string> codegen::buffer;
std::string codegen::argOrder[] = {"rdi", "rsi", "rdx", "r10", "r8", "r9"};
std::vector<std::string> codegen::availableRegisters = {"ax", "cx", "dx", "si", "di", "8", "9", "10", "11", "13", "14", "15"};
std::map<std::string, int> codegen::offsets;
std::map<std::string, std::string> codegen::tempToReg;

void codegen::writeLine(translator::instruction inst){
    switch(inst.type){
        case translator::assign:
            break;
        case translator::expression:// ~~~~~~~ Expression
            break;
        case translator::function:{// ~~~~~~~~ Function
            buffer.push_back("push %rbp");
            buffer.push_back("movq %rsp, %rbp");
            offsets = std::map<std::string, int>();
            int off = 0;
            for(int arg = 0; arg < inst.args.size(); arg += 2){
                std::string id = inst.args[arg + 1];
                offsets.insert(std::pair<std::string, int>(id, off));
                buffer.push_back("movl %" + argOrder[arg/2] + ", " + std::to_string(off) + "(%rbp)");
                off -= stoi(inst.args[arg]); 
            }
        }break;
        case translator::label:
            buffer.push_back(".globl " + inst.args[0]);
            buffer.push_back(inst.args[0] + ":");
            break;
    }
}

void codegen::outputToFile(std::string fileName){
    std::ofstream ofile;
    ofile.open(fileName);
    for(std::string s : codegen::buffer)
        ofile << s << "\n";
    ofile.close();
}
