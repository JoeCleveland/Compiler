#ifndef LEXICAL_H
#define LEXICAL_H

#include <string>
#include <vector>

typedef struct token {
    std::string lexeme;
    int type;
} token;

/**
 * Takes program as a string and returns
 * an array of lexemes
 **/
std::vector<token> lexAnalysis(std::string input);

std::vector<std::string> tokenize(std::string input);

#endif
