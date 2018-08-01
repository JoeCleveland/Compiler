#include "lexical.h"
#include "consts.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <vector>

std::string delimeters[] = {"++", "--", "==", "+=", "-=", "*", "/", "+", "-", "&", "|", "=", ",", ".", ";", "{", "}", "(", ")"};  

std::string keywords[] = {"fn", "met", "if", "else", "elif", "for", "while", "ret", "class", "int", "float"};

std::vector<std::string> tokenize(std::string input){
    std::vector<std::string> output = std::vector<std::string>();    
    
    int tokStart = 0;
    for(int i = 0; i < input.length(); i++){
         if((input.at(i) == ' ' || input.at(i) == '\n' || input.at(i) == '\t')
                 && (i - tokStart) > 0){
             output.push_back(input.substr(tokStart, i - tokStart));
             tokStart = i + 1;
         } else if(input.at(i) == ' ' || input.at(i) == '\n' || input.at(i) == '\t'){
             tokStart++;
         }
         else{
            for(int j = 0; j < NUMB_DELIMETERS; j++){
                if(delimeters[j].compare(input.substr(i, delimeters[j].length())) == 0){
                    if(tokStart < i)
                        output.push_back(input.substr(tokStart, i - tokStart));
                    output.push_back(delimeters[j]);
                    tokStart = i + delimeters[j].length();
                    if(delimeters[j].length() == 2)
                        i++;
                    break;
                }
            }
         }           
         if(i == input.length() - 1)
             output.push_back(input.substr(tokStart, input.length() - tokStart));
    }
    return output;
}

grammar_type classify(std::string token){
    bool containsAlpha = false;
    bool containsNums = false;
    bool firstAlpha = false;
    for(int ch = 0; ch < token.length(); ch++){
        if(token[ch] >= 48 && token[ch] <= 57)
            containsNums = true;
        if((token[ch] >= 65 && token[ch] <= 98) ||
           (token[ch] >= 97 && token[ch] <= 122)){
            containsAlpha = true;
            if(ch == 0)
                firstAlpha = true;
       }
    } 
    if(containsNums && !containsAlpha)
        return INT_LIT;
    if(firstAlpha)
        return ID;
    return INVALID;
}   

std::vector<token> lexAnalysis(std::string input){
    std::vector<std::string> tokens = tokenize(input); 
    std::vector<token> output = std::vector<token>(tokens.size() + 1);

    for(int i = 0; i < tokens.size(); i++){
        output[i].lexeme = tokens[i];
        //Check if token is a delimeter
        for(int delim = 0; delim < NUMB_DELIMETERS; delim++){
            if(!tokens[i].compare(delimeters[delim])){
                output[i].type = delim + DELIM_START;
                goto nexttoken;
            }
        }
        //Check if token is a keyword
        for(int keywrd = 0; keywrd < NUMB_KEYWORDS; keywrd++){
            if(!tokens[i].compare(keywords[keywrd])){
                output[i].type = keywrd + KEYWORD_START;
                goto nexttoken;
            }
        }
        //Otherwise use classifier
        output[i].type = classify(tokens[i]); 
        nexttoken:;
    }
    output[output.size() - 1].type = END;
    return output;
}

