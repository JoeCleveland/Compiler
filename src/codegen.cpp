#include <fstream>
#include <iostream>
#include "codegen.h"

std::vector<std::string> codegen::buffer;
std::string codegen::argOrder[] = {"di", "si", "dx", "10", "8", "9"};
std::vector<std::string> codegen::availableRegisters = {"cx", "dx", "si", "di", "8", "9", "10", "11", "13", "14", "15"};
//std::vector<std::string> codegen::availableRegisters = {"14", "15"};
std::map<std::string, int> codegen::offsets;
int codegen::lastOffset;
int codegen::argCount = 0;
bool codegen::mainFunc = false;
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

codegen::location codegen::getLoc(std::string tempOrVar){
    if(tempOrVar.at(0) == '%')
        return tempToLoc[tempOrVar];
    else
        return location(stack, std::to_string(offsets[tempOrVar]) + "(%rbp)");
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
        if(valLoc.type == reg)
            availableRegisters.push_back(valLoc.code);
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
        if(availableRegisters.size() > 2){
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
            location destLoc = getLoc(inst.args[0]);
            if(destLoc.type == reg)
                buffer.push_back("movl " + fmtValue(inst.args[1]) + ",\t" + fmtValue(inst.args[0]));
            else{
                std::string rgstr = getReg();
                buffer.push_back("movl " + fmtValue(inst.args[1]) + ",\t" + fmtReg(rgstr, 4));
                buffer.push_back("movl " + fmtReg(rgstr, 4) + ",\t" + fmtValue(inst.args[0]));
                availableRegisters.push_back(rgstr);
            }
        }break;
        case translator::expression:{// ~~~~~~~~~~~~~~~~~~~~~~ Expression
            writeExpressionLine(inst);
        }break;
        case translator::params:{// ~~~~~~~~ Function params
            buffer.push_back("push %rbp");
            buffer.push_back("movq %rsp,\t%rbp");
            offsets = std::map<std::string, int>();
            int off = 0;
            for(int arg = 0; arg < inst.args.size(); arg += 2){
                std::string id = inst.args[arg + 1];
                offsets.insert(std::pair<std::string, int>(id, off));
                buffer.push_back("movl " + fmtReg(argOrder[arg/2], 4) + ",\t" + std::to_string(off) + "(%rbp)");
                off -= stoi(inst.args[arg]); 
            }
            if(off < 0)
                buffer.push_back("subq $" + std::to_string(-off) + ",\t %rsp");
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
        case translator::label:{// ~~~~~~~~~~~~ Label
            std::string labelName = inst.args[0];
            if(labelName == "main"){
                mainFunc = true;
                labelName = "start";
            }
            buffer.push_back(".globl " + labelName);
            buffer.push_back(labelName + ":");
        }break;
        case translator::call:{
            buffer.push_back("call " + inst.args[1]);
            tempToLoc.insert(std::pair<std::string, location>(inst.args[0], location(reg, "ax")));              
            argCount = 0;
        }break;
        case translator::callarg:{
            std::string arg = inst.args[0];
            if(arg.at(0) == '%')
                getTemp(arg);
            else
                buffer.push_back("movl " + fmtValue(arg) + ",\t" + fmtReg(argOrder[argCount], 4));
            argCount++;
        }break;
        case translator::ret:{
            if(mainFunc && getLoc(inst.args[0]).code != "di")
                buffer.push_back("movl " + fmtValue(inst.args[0]) + ",\t" + fmtReg("di", 4));
            else if(getLoc(inst.args[0]).code != "ax")
                buffer.push_back("movl " + fmtValue(inst.args[0]) + ",\t" + fmtReg("ax", 4));

            if(lastOffset < 0)
                buffer.push_back("addq $" + std::to_string(-lastOffset) + ",\t %rsp");
            buffer.push_back("popq %rbp");
            if(mainFunc){
                buffer.push_back("movq $0x2000001, %rax");
                buffer.push_back("syscall");
                mainFunc = false;
            }
            else
                buffer.push_back("retq");
        }break;
    }
}

void codegen::writeExpressionLine(translator::instruction inst){
    std::string arg1 = inst.args[1];
    std::string op   = inst.args[2];
    std::string arg2 = inst.args[3];

    location loc = getTemp(inst.args[0]);
    std::string regstr;
    bool usingTempReg = false;
    if(loc.type == reg)
        regstr = loc.code;
    else{
        regstr = getReg();
        usingTempReg = true;
    } 

    if(op != "/"){
        buffer.push_back("movl " + fmtValue(arg1) + ",\t" + fmtReg(regstr, 4));
        std::string command;
        if(op == "+") command = "addl ";
        else if(op == "-") command = "subl ";
        else if(op == "*") command = "imul ";
        else if(op == "&") command = "andl ";
        else if(op == "|") command = "orl ";
        else if(op == "==") command = "cmpl ";
        buffer.push_back(command + fmtValue(arg2) + ",\t" + fmtReg(regstr, 4));
        if(usingTempReg && op != "==")
            buffer.push_back("movl " + fmtReg(regstr, 4) + ",\t" + fmtLoc(loc));
    } else if (op == "/"){
        buffer.push_back("movl " + fmtValue(arg1) + ",\t" + fmtReg("ax", 4));
        buffer.push_back("cltd");
        buffer.push_back("idivl " + fmtValue(arg2));
        buffer.push_back("movl " + fmtReg("ax", 4) + ",\t" + fmtLoc(loc));
    } 
    if (op == "=="){
        buffer.push_back("sete %al");
        buffer.push_back("andb $1,\t%al");
        buffer.push_back("movzbl %al,\t" + fmtLoc(loc));
    }


}

void codegen::outputToFile(std::string fileName){
    std::ofstream ofile;
    ofile.open(fileName);
    for(std::string s : codegen::buffer)
        ofile << s << "\n";
    ofile.close();
}
