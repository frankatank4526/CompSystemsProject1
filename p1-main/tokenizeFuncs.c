#include "tokenizeFuncs.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>


//global variable for max input size
int MAX_INPUT_SIZE = 256;

// char -> boolean
// checks if given character is one of the character considered valid and not a special character
int is_valid_char(char ch) {
  // this relies on the fact that digits are ordered in the ASCII table
     return  ch >= '0' && ch <= '9' || (ch >= 'a' && ch <= 'z' || (ch >= 'A' && ch <= 'Z'))
 || ch == '_' || ch == '.' || ch == '/' || ch == '-' || ch == '~' || ch == '@' || ch == '#' || ch == '$' || ch == '^'
|| ch == '&' || ch == '*' || ch == '=' || ch == '+' || ch == '?';
     }

// char * -> boolean
// checks if there is a second end parenthese to group statement and if there are special
//characters within the group
bool parenedgechecker(const char *input) {
  int i = 0;
  int stop = 0;
  bool checker = false;
  while (input[i] != '\0' && stop != 10) {
    //checks for special characters
    if (input[i] == '<' || input[i] == '>' || input [i] == ';' || input[i] == '|') {
      checker = true;
    }
    //checks for end parenthese
    if (input[i + 1] == ')') {
      stop = 10;
      ++i;
    } else {
      ++i;
    }
  }
  return checker;
}

// char* -> int
// Function for grouping strings surrounded by quotation marks and outputting that group
int findsecondgrouper(const char *input, char *output) {
  int i = 0;
  int stop = 0;
  while (input[i] != '\0' && stop != 10) {
    //checks for end quotation mark
    if (input[i + 1] == '\"') {
      output[i] = input[i];
      stop = 10;
      ++i;
  } else {
    output[i] = input[i];
    ++i;
  }
  }
output[i] = '\0';
return i;
}

// char * -> int
// Function for grouping strings surrounded by parentheses and outputting that group
int parencase(const char *input, char *output) {
  int i = 0;
  int stop = 0;
  while (input[i] != '\0' && stop != 10) {
    //checks for end parenthese
    if (input[i + 1] == ')') {
      output[i] = input[i];
      stop = 10;
      ++i;
  } else {
    output[i] = input[i];
    ++i;
  }
  }
output[i] = '\0';
return i;
}

/**
 *  * Read the next integer as a string from the input into the output.
 *   */
int read_integer_string(const char *input, char *output) {
  int i = 0;
// while we have input and the character is valid,
 while (input[i] != '\0' && is_valid_char(input[i])) {
 output[i] = input[i]; // copy character to output buffer
++i;
}
  output[i] = '\0'; // add the terminating byte

  return i; // return the length of the string
}

// char ** -> char **
// Function to tokenize a given expression
char ** tokenize(char * expr){
	//checks if size of expression matches
        if(strlen(expr) > MAX_INPUT_SIZE){
        perror("input too big\n");
        }
 char ** listOfTokens = (char **) malloc(255 * sizeof(char*));
  int pipe_fd[2];
  int indexOfList = 0;
  int read_fd;// = pipe_fd[0];
  int write_fd;// = pipe_fd[1];
  int i = 0; // current position in string
  char buf[256];
  char listInputBuffer[256];
  int size;
    while (expr[i] != '\0') {
    // while the end of string is not reached
    // first check if the current char is a valid character
    //
         if (is_valid_char(expr[i])) {
    // read the integer from the output AND
    // advance the current position by the length of the string
    size = read_integer_string(&expr[i], buf);
    i+= size;
   listOfTokens[indexOfList] = (char *)  malloc(sizeof(buf));
    strcpy(listOfTokens[indexOfList], buf);
    indexOfList++;
    continue; // skip the rest of this iteration
     }
         else {
// special quotation mark case
if (expr[i] == '\"') {
i++;
i += findsecondgrouper(&expr[i], buf);
i++;
  listOfTokens[indexOfList] = (char *) malloc(sizeof(char) * size);
  strcpy(listOfTokens[indexOfList], buf);
indexOfList++;
continue;
}
// special parenthese case
if (expr[i] == '(' && !parenedgechecker(&expr[i])) {
i++;
i += parencase(&expr[i], buf);
i++;
  listOfTokens[indexOfList] = (char *) malloc(sizeof(char) * size);
  strcpy(listOfTokens[indexOfList], buf);
  indexOfList++;
continue;
}
}
// if not number, consider the current character and print its type
   switch (expr[i]) {
      case ';':
  listOfTokens[indexOfList] = (char *) malloc(sizeof(char) * size);
  strcpy(listOfTokens[indexOfList], ";");
  indexOfList++;
      break;
      case '<':
    listOfTokens[indexOfList] = (char *) malloc(sizeof(char) * size);
    strcpy(listOfTokens[indexOfList], "<");
    indexOfList++;
        break;
      case '>':
     listOfTokens[indexOfList] = (char *) malloc(sizeof(char) * size);
    strcpy(listOfTokens[indexOfList], ">");
    indexOfList++;
        break;
      case '|':
    listOfTokens[indexOfList] = (char *) malloc(sizeof(char) * size);
    strcpy(listOfTokens[indexOfList], "|");
    indexOfList++;
        break;
      case '\'':
        break;
      case '\"':
    listOfTokens[indexOfList] = (char *) malloc(sizeof(char) * size);
    strcpy(listOfTokens[indexOfList], "\"");
    indexOfList++;
       break;
      case '(':
  listOfTokens[indexOfList] = (char *) malloc(sizeof(char) * size);
  strcpy(listOfTokens[indexOfList], "(");
  indexOfList++;
       break;
      case ')':
  listOfTokens[indexOfList] = (char *) malloc(sizeof(char) * size);
  strcpy(listOfTokens[indexOfList], ")");
  indexOfList++;
      break;
      case ' ': // skip spaces
      break;
   default:
   printf("");
   }
++i;
}
return listOfTokens;
}

// char ** -> void
// frees give token list data
void freeTokenList(char ** tokenList){
       int i = 0;
    while(tokenList[i] != NULL){
      free(tokenList[i]);
      i++;   
    }
    free(tokenList);
}



