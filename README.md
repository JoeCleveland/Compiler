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

The first stage of compilation is lexical analysis. In this stage code is split into 'lexemes' by delimeters and then assigned a token. A lexeme is a string representing the smallest piece a program can be split into, such as keywords, operators, or identifiers. A token is an integer specifying the type of that lexeme. Delimeters include spaces, tabs, newlines, and the symbols specified in the array ```std::string delimiters[]``` in lexical.cpp. The function lexAnalysis takes in the entire program as a string and returns it as a vector of tokens. Each token stores the original lexeme that makes up the token, and an integer type. All of the possible token types are defined in consts.h.
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
We specify the grammar in Backus-Naur form, which is a set of relations from non-terminals, which are overarching grammatical structures, to more specific structures and terminals (tokens).
As a simple example, the following is a grammar for parsing simple expressions.
```
EXPRESION -> EXPRESSION + TERM | TERM
TERM      -> TERM * FACTOR | FACTOR
FACTOR    -> VALUE | ID
```
With this grammar, parsing the expression 5 + x * y would produce the following parse tree:
```
            EXPRESION
                |
     EXPRESSION + TERM  
     /               \               
  TERM          TERM * FACTOR         
   |            /         \
 FACTOR       FACTOR      ID (y)
   |             |
 VALUE (5)      ID (x)
```
In this example, EXPRESSIONs, TERMs, and FACTORs are non-terminals, and VALUEs, IDs and the symbols (+, \*) are all terminals.

The parser used in this project is a recursive-descent parser. Each non-terminal in the grammar corresponds to a function of the parser. Within each relation of the grammar, the non-terminals on the right side of the relation correspond to calls to the corresponding functions, some of these calls being recursive. A parse tree is not explicitly built as an actual data structure but is built on the stack as a side-effect of the function calls. 

Since we are using a recursive descent parser, a simple grammar such as the one above cannot be used, since it contains left-recursion which would cause the parser to get stuck trying to parse the leftmost non-terminal.
Here is the actual grammar for parsing expressions in this project, which eliminates left recursion:
(The curly braces mean that piece of the relation can be repeated indefinitley)
```
EXPRESSION  -> AND_TERM { | AND_TERM }
AND_TERM    -> EQ_TERM { & EQ_TERM }
EQ_TERM     -> ADDSUB_TERM { == ADSUM_TERM }
ADDSUB_TERM -> MULDIV_TERM { + or - MULDIV_TERM }
MULDIV_TERM -> VALUE_TERM { * or / VALUE_TERM }
VALUE_TERM  -> id | INT_LIT | FLOAT_LIT | CALL
```
