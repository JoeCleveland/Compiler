#include <fstream>
#include <iostream>
#include "codegen.h"

std::vector<std::string> codegen::buffer;
std::string codegen::argOrder[] = {"di", "si", "dx", "10", "8", "9"};
std::vector<std::string> codegen::availableRegisters = {"cx", "dx", "si", "di", "8", "9", "10", "11", "13", "14", "15"};
std::map<std::string, int> codegen::offsets;
int codegen::lastOffset;
std::map<std::string, codegen::location> codegen::tempToLoc;

std::string codegen::fmtReg(std::string reg, int bytes) {
    if(reg.back() == 'x' || reg.back() == 'i'){//If 32-bit style register
        switch(bytes){
            case 4:
                return "%e" + reg;
            case 8:
                return "%r" + reg;
        }
    }    
    else
        return "%r" + reg + "d";
}

std::string codegen::fmtLoc(location loc){
    if(loc.type == reg)
        return fmtReg(loc.code, 4);
    else 
        return loc.code;
}

std::string codegen::fmtValue(std::string val){
    if(val.at(0) == '%'){
        location valLoc = tempToLoc[val];
        return fmtLoc(valLoc);
    } 
    else if (val.at(0) == '$')
        return val;
    else
        return std::to_string(offsets[val]) + "(%rbp)";
}

codegen::location codegen::getTemp(std::string temp){
    if(tempToLoc.count(temp))//If temp exists
        return tempToLoc[temp];
    else{
        location newLoc;
        if(availableRegisters.size() > 1){
            newLoc.type = reg;
            newLoc.code = getReg(); 
        }
        else{
            newLoc.type = stack;
            newLoc.code = std::to_string(lastOffset) + "(%rbp)";
        }
        tempToLoc.insert(std::pair<std::string, location>(temp, newLoc));
        return newLoc;
    }
}

std::string codegen::getReg() {
    std::string rgstr = availableRegisters.back();
    availableRegisters.pop_back();
    return rgstr;
}

void codegen::writeLine(translator::instruction inst){
    switch(inst.type){
        case translator::assign:{// ~~~~~~~~~~~~~~~~~~~ Assign
        }break;
        case translator::expression:{// ~~~~~~~~~~~~~~~~~~~~~~ Expression
            writeExpressionLine(inst);
        }break;
        case translator::params:{// ~~~~~~~~ Function params
            buffer.push_back("push %rbp");
            buffer.push_back("movq %rsp, %rbp");
            offsets = std::map<std::string, int>();
            int off = 0;
            for(int arg = 0; arg < inst.args.size(); arg += 2){
                std::string id = inst.args[arg + 1];
                offsets.insert(std::pair<std::string, int>(id, off));
                buffer.push_back("movl " + fmtReg(argOrder[arg/2], 4) + ", " + std::to_string(off) + "(%rbp)");
                off -= stoi(inst.args[arg]); 
            }
            lastOffset = off;
        }break;
        case translator::vars:{// ~~~~~~~~~~~ Function vars
            int off = lastOffset;
            for(int arg = 0; arg < inst.args.size(); arg += 2){
                std::string id = inst.args[arg + 1];
                offsets.insert(std::pair<std::string, int>(id, off));
                off -= stoi(inst.args[arg]); 
            }
        }break;
        case translator::label:// ~~~~~~~~~~~~ Label
            buffer.push_back(".globl " + inst.args[0]);
            buffer.push_back(inst.args[0] + ":");
            break;
        case translator::call:{
            buffer.push_back("call " + inst.args[1]);
            //tempToReg.insert(std::pair<std::string, std::string>(inst.args[0], "ax"));              
        }break;
        case translator::callarg:{
        }break;
        case translator::ret:{
        }break;
    }
}

void codegen::writeExpressionLine(translator::instruction inst){
    location loc = getTemp(inst.args[0]);
    std::string regstr;
    bool usingTempReg = false;
    if(loc.type = reg)
        regstr = loc.code;
    else{
        regstr = getReg();
        usingTempReg = true;
    }

    std::string arg1 = inst.args[1];
    char op   = inst.args[2].at(0);
    std::string arg2 = inst.args[3];
    if(op != '/'){
        buffer.push_back("movl " + fmtValue(arg1) + ",\t" + fmtReg(regstr, 4));
        std::string command;
        if(op == '+') command = "addl ";
        if(op == '-') command = "subl ";
        if(op == '*') command = "imul ";
        buffer.push_back(command + fmtValue(arg2) + ",\t" + fmtReg(regstr, 4));
    } else {
    }
    if(usingTempReg)
        buffer.push_back("movl " + fmtReg(regstr, 4) + ",\t" + fmtLoc(loc));

}

void codegen::outputToFile(std::string fileName){
    std::ofstream ofile;
    ofile.open(fileName);
    for(std::string s : codegen::buffer)
        ofile << s << "\n";
    ofile.close();
}
