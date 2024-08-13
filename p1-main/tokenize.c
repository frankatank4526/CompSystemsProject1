#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include "tokenizeFuncs.h"

int main(int argc, char **argv) {
    if (argc < 1) //checks if there are arguments
      return 0;
    char expr[256];
    int length = read(0, expr, 255); //reads in arguments
    expr[length] = '\0'; //adds end token to end of argument string
    char ** output = tokenize(expr); //calls tokenize function from tokenizeFuncs and gives it output array
    int i = 0;
    //prints out array received from tokenize
    while(output[i] != NULL){
      printf("%s\n", output[i]);
      i++;
    }
    //frees output array data
    freeTokenList(output);

return 0;
}

