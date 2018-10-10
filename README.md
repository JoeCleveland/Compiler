# An overview of this project

This is a compiler for a C like language.
The design of this language is incomplete, but it will be expanded as soon as the basic implementation of this compiler is finished.
The exent of valid syntax understood by the compiler is shown in this code sample:
```
fn (int) doSomething (int a, int b)
  int c = a - b;
  if a == b | a + 1 == b{
    ret a + b * c;
  }
  ret b / c;
}

fn main {
  ret doSomething(1, 3, 4);
}
```
## Stages of the Compiler

### Lexical Analysis

The first stage of compilation is lexical analysis. In this stage code is split into 'lexemes' by delimeters and then assigned a token. A lexeme is a string representing the smallest piece a program can be split into, such as keywords, operators, or identifiers. A token is an integer specifying the type of that lexeme. Delimeters include spaces, tabs, newlines, and the symbols specified in the array ```std::string delimiters[]``` in lexical.cpp. The function lexAnalysis takes in the entire program as a string and returns it as a vector of tokens. Each token stores the original lexeme that created that makes up the token, and an integer type. All of the possible token types are defined in consts.h.
```
typedef struct token {
    std::string lexeme;
    int type;
} token;
```
For example, the following input string:
```
int myInt = 50;
```
Would be split into the following token types:
```
INT ID EQ INT_LIT SEMI
```
Notice that keywords and symbols have their own token types (INT, EQ, SEMI), whereas identifiers and literals are assigned a generic token type. The identifiers and literals are not lost however, as we are storing the lexemes as well.
Splitting the program into a sequence of tokens makes the process of parsing much simpler.

### Parsing

Parsing is the process of sequentially looking through the tokens and building a parse tree based on grammar of the language.
We specify the grammar in Backus-Naur form, which is a set of relations from non-terminals to terminals and other non-terminals.
