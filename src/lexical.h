#ifndef LEXICAL_H
#define LEXICAL_H

#include <string>

typedef struct token {
    std::string lexeme;
    int type;
} token;

/**
 * Takes program as a string and returns
 * an array of lexemes
 **/
std::vector<token> lexAnalysis(std::string input);

#endif
