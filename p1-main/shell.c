// 256 assuming a '\0' char needs to be added at end
//#define <MAX_INPUT_SIZE>  <256>

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

// Executes a given list of strings (tokens)
// Handles all special cases (redirection, pipe, sequencing)
// takes in a list of strings, doesn't return anything
void executeTokens(char ** tokens){
          int j = 0;
    int lastSemi = 0;
     int hasSemi = 0;
    // Checks tokens for special characters          
    while(tokens[j] != NULL){
      // handles sequencing
      // same logic for each special character; upon detecting special token,
      // split tokens into "command before token" and "command after token" 
      // "command before token" = scriptTokens, "command after token" = scriptTokens2"
      // After splitting, each are individually executed appropriately
      if(strcmp(";", tokens[j]) == 0){
        char ** scriptTokens = (char **) malloc(256 * sizeof(char *));
        int k = 0;
        hasSemi = 1;
	lastSemi = j;

    while(strcmp(";", tokens[k]) != 0 ){
      scriptTokens[k] = tokens[k];
      k++;
    }

      executeTokens(scriptTokens);
      char ** scriptTokens2 = (char **) malloc(256 * sizeof(char *));
       k++;
      int i = 0;
      while(tokens[k] != NULL){
        scriptTokens2[i] = tokens[k];
        k++;
	i++;
    }
       executeTokens(scriptTokens2);
       free(scriptTokens);
       free(scriptTokens2);
      break;
      }
      j++;



    } 
     j = 0;
    
     int hasRedirectLeft = 0;
    // handles redirection
    while(tokens[j] != NULL){
      if(strcmp("<", tokens[j]) == 0){
        char ** scriptTokens = (char **) malloc(256 * sizeof(char *));
        int k = 0;
        hasRedirectLeft = 1;
    

    while(strcmp("<", tokens[k]) != 0 ){
      scriptTokens[k] = tokens[k];
      k++;
    }
      
        

      char ** scriptTokens2 = (char **) malloc(256 * sizeof(char *));
       k++;
      int i = 0;
      while(tokens[k] != NULL){
        scriptTokens2[i] = tokens[k];
        k++;
        i++;
    }
        
      int pid = fork();
      if(pid == 0){
        close(0);
	if(open(scriptTokens2[0], O_RDONLY) != 0){

          printf("Unable to set input file\n");
	  exit(1);
	}
	executeTokens(scriptTokens);
        exit(0);
      
      }
      wait(NULL);
       free(scriptTokens);
       free(scriptTokens2);
      break;
      }
      j++;



    }

    j = 0;
    int hasRedirectRight = 0;

    while(tokens[j] != NULL){
      // handles redirection 	    
      if(strcmp(">", tokens[j]) == 0){
        char ** scriptTokens = (char **) malloc(256 * sizeof(char *));
        int k = 0;
        hasRedirectRight = 1;


    while(strcmp(">", tokens[k]) != 0 ){
      scriptTokens[k] = tokens[k];
      k++;
    }




      char ** scriptTokens2 = (char **) malloc(256 * sizeof(char *));
       k++;
      int i = 0;
      while(tokens[k] != NULL){
        scriptTokens2[i] = tokens[k];
        k++;
        i++;
    }



      int pid = fork();
      if(pid == 0){
        close(1);
        if(open(scriptTokens2[0], O_WRONLY | O_CREAT, 0666) != 1){
          printf("Unable to set output file\n");
	  exit(1);
	}
 
       executeTokens(scriptTokens);
        exit(0);

      }
      wait(NULL);
      free(scriptTokens);
      free(scriptTokens2);
      break;
      }
      j++;



    }
    j = 0;
    int hasPipe = 0;

    while(tokens[j] != NULL){
      // handles pipe
      if(strcmp("|", tokens[j]) == 0){
        char ** scriptTokens = (char **) malloc(256 * sizeof(char *));
        int k = 0;
        hasPipe = 1;


    while(strcmp("|", tokens[k]) != 0 ){
      scriptTokens[k] = tokens[k];
      k++;
    }



      //executeTokens(scriptTokens);
      char ** scriptTokens2 = (char **) malloc(256 * sizeof(char *));
       k++;
      int i = 0;
      while(tokens[k] != NULL){
        scriptTokens2[i] = tokens[k];
        k++;
        i++;
    }
     int pid = fork();
      if(pid == 0){
        int pipe_fd [2];
        pipe(pipe_fd);
        int read_fd = pipe_fd[0];
        int write_fd = pipe_fd[1];
        int pid2 = fork();
 	if (pid2 == 0){
          close(1);
          close(read_fd);
          dup2(write_fd, 1);
          executeTokens(scriptTokens);
	  exit(0);
	}
	else{
	 wait(NULL);	
         close(write_fd);

         close(0);
         dup(read_fd);
         executeTokens(scriptTokens2);
         exit(0);
	}

      }
       
      wait(NULL);
       free(scriptTokens);
       free(scriptTokens2);
      break;
      }
      j++;

    }


    if(hasSemi == 0 && hasRedirectLeft == 0 && hasRedirectRight == 0 && hasPipe == 0){

      int child_pid = fork();

      if(child_pid == 0){

        if(execvp(tokens[0],  tokens) == -1){
          printf("[%s]: command not found\n", tokens[0]);
          exit(1);

        }

      }


      else if(child_pid > 0){
        wait(NULL);
        
      }

      else {

         perror("Error: fork failed\n");
         exit(1);



    }
    }

}


// Shell main method
// Executes infinite loop for mini shell
// Does not take any inputs given with launch command "e.g. ./shell arg1 arg2" 
int main(int argc, char **argv) {
  printf("Welcome to mini-shell.\n");

char* prevcommand = (char *) malloc(MAX_INPUT_SIZE * sizeof(char));

  while(1){
    printf("shell $ ");
    char* command = (char *) malloc(MAX_INPUT_SIZE * sizeof(char));
    
    char* input = fgets(command, 256, stdin);

    int isAllSpaces = 1;
    // handles control d
    if(input == NULL){
      printf("\nBye bye.\n");
      break;

    }
    char ** tokens = tokenize(command);
    // no arguments given, retrieve input again
    if(tokens[0] == NULL){
       continue;
    }


    // checks if the command is all spaces 
    int i = 0;
    while(tokens[i] != NULL){
      if(strcmp(" ", tokens[i]) != 0)
             isAllSpaces = 0;
      i++;

    }
    // handles command "prev
    if (strcmp("prev", tokens[0]) == 0) {
    if (prevcommand[0] == '\0') {
    printf("No previous command to execute\n");
   continue;
   } else {
    printf("%s", prevcommand);
    tokens = tokenize(prevcommand);
    }
} else {
   prevcommand = command;
}
    // handles command "exit" 
    if(strcmp("exit", tokens[0]) == 0){
       freeTokenList(tokens);
       free(command);
       printf("Bye bye.\n");
       break;
    }
   // handles command "help"
   if (strcmp("help", tokens[0]) == 0) {
      printf("cd (change directory): This command should change the current working directory of the shell to the path specified as the argument.\n\n");
      printf("source: Execute a script. Takes a filename as an argument and processes each line of the file as a command, including built-ins. \n\n");
      printf("prev: Prints the previous command line and executes it again, without becomming the new command line.\n\n");
      printf("help: Explains all the built-in commands available in your shell\n\n");
      printf("exit: Exits the shell\n\n");
      continue;
 }

             // changes directory upon detecting "cd" token, outputs error upon
        // incorrect args or non existent directory
    if(strcmp("cd", tokens[0]) == 0){
      if(tokens[1] == NULL || (chdir(tokens[1]) != 0)){
        printf("Error: incorrect arguments to cd, or no such directory\n");

        continue;

      }
      continue;


    }
    // handles source command 
    if(strcmp("source", tokens[0]) == 0){
      if(tokens[1] == NULL){
        printf("Error: no file provided\n");
        continue;
      }

      else{
        FILE* file = fopen(tokens[1], "r");
        if(file == NULL){
        printf("Error: invalid script file\n");
	continue;

	
	}	


	else{
	  
          char* inputLine = "";
	  char* linesRead = (char *) malloc(MAX_INPUT_SIZE * sizeof(char));

	  while(1){
	    inputLine = fgets(linesRead, 255, file);
	    if(inputLine == NULL){
                
		break;
	    }
            int pid = fork();
            char ** scriptTokens = tokenize(linesRead);
	    if(pid == 0){
	      


              if(execvp(scriptTokens[0], scriptTokens) == -1){
                printf("Error executing line\n");
		exit(1);
	      }      
              
               
	      
	    }
	    else if(pid > 0){
              wait(NULL);
	      
	      continue;  

	    }

	    else{
	      perror("fork failed\n");
	      exit(1);
	      



	  }
	    freeTokenList(scriptTokens);
	}
	  free(linesRead);
	  
      }




    }
      continue;
    }
    // if command isn't all spaces, execute the command. 
    // otherwise, free allocated memory and go to next iteration. 	
    if(isAllSpaces == 0){

     executeTokens(tokens);
    }
    else{
      freeTokenList(tokens);
      free(command);
      continue;

    }





  }
return 0;
}
