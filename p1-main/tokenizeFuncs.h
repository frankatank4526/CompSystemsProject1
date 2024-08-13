#ifndef _TOKENIZEFUNCS_H
#define _TOKENIZEFUNCS_H
#include <stdbool.h>

extern int MAX_INPUT_SIZE;

int is_valid_char(char ch);

bool paranedgechecker(const char *input);

int findsecondgrouper(const char *input, char *output);

int parencase(const char *input, char *output);

int read_integer_string(const char *input, char *output);

char ** tokenize(char * expr);

void freeTokenList(char ** tokenList);

















#endif
