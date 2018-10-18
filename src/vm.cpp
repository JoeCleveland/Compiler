
#include "vm.h"
#include <iostream>
#include <string>
#include <map>

#define FRAME frames.back()

typedef std::map<std::string, int> vm_vars;
typedef std::pair<std::string, int> vm_var;
typedef struct stack_frame {
    vm_vars vars;
    vm_vars params;
} stack_frame;

int IP;
std::vector<translator::instruction> code;
std::vector<stack_frame> frames;
std::vector<int> callParams;
std::string returnLoc;
int callLocation;

int resolveValue(std::string varOrVal){
    std::string sub = varOrVal.substr(1, varOrVal.length());
    if(varOrVal[0] == '$')
        return stoi(sub);
    else
        return FRAME.vars[varOrVal];
}

void jumpTo(std::string label){
    IP = 0;
    bool found = false;
    while(!found){
        if(code.at(IP).type == translator::label &&
           code.at(IP).args[0] == label)
            found = true;
        else    
            IP++;
    }
}

void execAssign(translator::instruction inst){
    int value = resolveValue(inst.args[1]);
    std::string dest = inst.args[0];
    FRAME.vars[dest] = value;
}

void execExpression(translator::instruction inst){
    std::string tempName = inst.args[0];
    int leftArg = resolveValue(inst.args[1]);
    int rightArg = resolveValue(inst.args[3]);
    int result;
    if(inst.args[2] == "+")
        result = leftArg + rightArg;
    else if(inst.args[2] == "-")
        result = leftArg - rightArg;
    else if(inst.args[2] == "*")
        result = leftArg * rightArg;
    else if(inst.args[2] == "/")
        result = leftArg / rightArg;
    else if(inst.args[2] == "==")
        result = leftArg == rightArg;
    else if(inst.args[2] == "&")
        result = leftArg & rightArg;
    else if(inst.args[2] == "|")
        result = leftArg | rightArg;
    FRAME.vars.insert(vm_var(tempName, result));
    std::cout << "VM: " << tempName << ":" << leftArg << ", " << rightArg << ":" << result << "\n"; 
}

/**
 * Adds the variables in callParams to the stack frame 
 **/
void execParams(translator::instruction inst){
    for(int param = 0; param < inst.args.size(); param += 2){
        std::string varName = inst.args[param + 1];
        int val = callParams.back();
        callParams.pop_back();
        FRAME.vars.insert(vm_var(varName, val));
    }
}

/**
 * Adds variables to current stack frame
 **/
void execVars(translator::instruction inst){
    for(int param = 0; param < inst.args.size(); param += 2){
        std::string varName = inst.args[param + 1];
        FRAME.vars.insert(vm_var(varName, 0));
    }
}

/**
 * Executes call and creates new stack frame
 **/
void execCall(translator::instruction inst){
    returnLoc = inst.args[0]; 
    FRAME.vars.insert(vm_var(returnLoc, 0));
    callLocation = IP;
    jumpTo(inst.args[1]);
    stack_frame sf;
    frames.push_back(sf);
}

void execCallArg(translator::instruction inst){
    int val = resolveValue(inst.args[0]);
    callParams.insert(callParams.begin(), val);
}

void execRet(translator::instruction inst){
    int retVal = resolveValue(inst.args[0]);
    std::cout << "vm: RETURN " << retVal << std::endl;
    if(frames.size() > 1){
        frames.pop_back();
        IP = callLocation;
    }
    FRAME.vars[returnLoc] = retVal;
}

void execCondJump(translator::instruction inst){
    int value = resolveValue(inst.args[0]);
    if (value == 0)
        jumpTo(inst.args[1]);
}

void execJump(translator::instruction inst){
    jumpTo(inst.args[0]);
}

void execute(translator::instruction inst){
    switch(inst.type){
        case translator::assign: execAssign(inst); break;
        case translator::expression: execExpression(inst); break;
        case translator::params: execParams(inst); break;
        case translator::vars: execVars(inst); break;
        case translator::call: execCall(inst); break;
        case translator::callarg: execCallArg(inst); break;
        case translator::ret: execRet(inst); break;
        case translator::condjump: execCondJump(inst); break;
        case translator::jump: execJump(inst); break;
    } 
}

void vm::runProgram(std::vector<translator::instruction> inputcode){
    code = inputcode;
    bool running = true;
    IP = 0;
    stack_frame sf;
    frames.push_back(sf);
    jumpTo("main");
    while(running){
        translator::instruction inst = code.at(IP);
        
        execute(inst);
        IP++;
        if(IP >= code.size())
            running = false;
    }
}
