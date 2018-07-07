#ifndef CONSTS_H
#define CONSTS_H

#include <string>

extern std::string delimeters[]; 
#define NUMB_DELIMETERS 19 

extern std::string keywords[];
#define NUMB_KEYWORDS 11 
typedef int grammar_type;

//types for data_type struct
#define TYPE_USER 0
#define TYPE_PRIM_INT 1
#define TYPE_PRIM_FLOAT 2

typedef struct data_type {
    int primitive; //TYPE_USER, TYPE_INT etc..
    int tableIndex; //Index in global table where user type is defined
    data_type(int p, int t):primitive(p), tableIndex(t) {}
} data_type;

//non keyword/symbolic types
#define INVALID -2
#define END -1

#define ID 0
#define INT_LIT 1
#define FLOAT_LIT 2

//Delimiter symbols:
#define DELIM_START 3
#define INC 3
#define DEC 4
#define EQEQ 5
#define PLUS_EQ 6 
#define MINUS_EQ 7 
#define MULT 8 
#define DIV 9 
#define PLUS 10 
#define SUB 11 
#define AND 12 
#define OR 13 
#define EQ 14 
#define COMMA 15 
#define PERIOD 16 
#define SEMI 17 
#define O_BRACKET 18 
#define C_BRACKET 19 
#define O_PAREN 20 
#define C_PAREN 21 

//Keyword symbols:
#define KEYWORD_START 22
#define FN 22
#define MET 23
#define IF 24
#define ELSE 25
#define ELIF 26
#define FOR 27
#define WHILE 28
#define RET 29
#define CLASS 30
#define INT 31
#define FLOAT 32

#endif
