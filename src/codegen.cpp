#include <fstream>
#include <iostream>
#include "codegen.h"

std::vector<std::string> codegen::buffer;
std::string codegen::argOrder[] = {"di", "si", "dx", "10", "8", "9"};
std::vector<std::string> codegen::availableRegisters = {"cx", "dx", "si", "di", "8", "9", "10", "11", "13", "14", "15"};
std::map<std::string, int> codegen::offsets;
std::map<std::string, std::string> codegen::tempToReg;

std::string fmtReg(std::string reg, int bytes) {
    if(reg.back() == 'x' || reg.back() == 'i'){//If 32-bit style register
        switch(bytes){
            case 4:
                return "e" + reg;
            case 8:
                return "r" + reg;
        }
    }    
    else
        return "r" + reg + "d";
}

void codegen::writeLine(translator::instruction inst){
    switch(inst.type){
        case translator::assign:{// ~~~~~~~~~~~~~~~~~~~ Assign
            std::string dest = std::to_string(offsets[inst.args[0]]) + "(%rbp)";
            std::string src;
            char srcType = inst.args[1].at(0);
            if(srcType == '%')
                src = "%" + fmtReg(tempToReg[inst.args[1]], 4); 
            else if(srcType == '$')
                src = inst.args[1];
            else{
                std::string reg = availableRegisters.back();
                std::string off = std::to_string(offsets[inst.args[1]]); 
                buffer.push_back("movl " + off + "(%rbp)" + ", " + fmtReg(reg, 4));
                src = "%" + fmtReg(tempToReg[inst.args[1]], 4); 
            }
            buffer.push_back("movl " + src + ", " + dest);
        }break;
        case translator::expression:{// ~~~~~~~~~~~~~~~~~~~~~~ Expression
            std::string reg; //Register where result is stored
            std::string src; //Stack loc or register of var2
            std::string var1 = inst.args[1];
            std::string op   = inst.args[2];
            std::string var2 = inst.args[3];
            //if first var is a tempval reuse its register for new temp
            if(var1.at(0) == '%'){
                reg = tempToReg[var1];
                tempToReg.insert(std::pair<std::string, std::string>(inst.args[0], reg));
            } else {
                //otherwise move variable from stack to new register
                reg = availableRegisters.back();
                availableRegisters.pop_back();
                tempToReg.insert(std::pair<std::string, std::string>(inst.args[0], reg));
                std::string stackLoc = std::to_string(offsets[var1]) + "(%rbp)";
                buffer.push_back("movl " + stackLoc + ", %" + fmtReg(reg, 4));
            }
            //set src to location or register:
            if(var2.at(0) == '%')
                src = "%" + fmtReg(tempToReg[var2], 4);
            else
                src = std::to_string(offsets[var2]) + "(%rbp)";
            //generate operation:
            if(op == "+")
                buffer.push_back("addl " + src + ", %" + fmtReg(reg, 4));
            else if(op == "-")
                buffer.push_back("subl " + src + ", %" + fmtReg(reg, 4));
            else if(op == "*")
                buffer.push_back("imull " + src + ", %" + fmtReg(reg, 4));
            else if(op == "/"){
                buffer.push_back("movl " + src + ", %" + fmtReg("ax", 4));
                buffer.push_back("cltd");
                buffer.push_back("idivl %" + fmtReg(reg, 4));
            }

        }break;
        case translator::function:{// ~~~~~~~~ Function
            buffer.push_back("push %rbp");
            buffer.push_back("movq %rsp, %rbp");
            offsets = std::map<std::string, int>();
            int off = 0;
            for(int arg = 0; arg < inst.args.size(); arg += 2){
                std::string id = inst.args[arg + 1];
                offsets.insert(std::pair<std::string, int>(id, off));
                buffer.push_back("movl %" + fmtReg(argOrder[arg/2], 4) + ", " + std::to_string(off) + "(%rbp)");
                off -= stoi(inst.args[arg]); 
            }
        }break;
        case translator::label:// ~~~~~~~~~~~~ Label
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
