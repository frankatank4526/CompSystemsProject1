/**
 *  * A simple arithmetic expression tokenizer. Use this example to get you 
 *   * started on the first task.
 *    */

#include <stdio.h>
#include <stdbool.h>
/**
 *  * Is the given character a digit?
 *   */

int is_digit(char ch) {
  // this relies on the fact that digits are ordered in the ASCII table
     return ch >= '0' && ch <= '9' || (ch >= 'a' && ch <= 'z' || (ch >= 'A' && ch <= 'Z')); 
     }

int findsecondgrouper(const char *input, char *output) {
int i = 0;
int stop = 0;
while (input[i] != '\0' && stop != 10) {
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

int parencase(const char *input, char *output) {
int i = 0;
int stop = 0;
while (input[i] != '\0' && stop != 10) {
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
// while we have input and the character is a digit,
 while (input[i] != '\0' && is_digit(input[i])) {
 output[i] = input[i]; // copy character to output buffer
++i;
}

  output[i] = '\0'; // add the terminating byte

  return i; // return the length of the string
}

int main(int argc, char **argv) {
    if (argc < 1)
      return 0;
    char expr[256];
    int length = read(0, expr, 255);
    expr[length] = '\0';
//  char expr[] = "this < is > a demo (noice) \"This is a sentence\" ; \"some ( special > chars\""; // example expression string
 int i = 0; // current position in string
char buf[256];
  while (expr[i] != '\0') { // while the end of string is not reached
    // first check if the current char is a digit
         if (is_digit(expr[i])) {
    // read the integer from the output AND
    // advance the current position by the length of the string
    i += read_integer_string(&expr[i], buf); 
    //                             // could convert it here...
    printf("%s\n", buf);
    continue; // skip the rest of this iteration
     } else {
if (expr[i] == '\"') {
i++;
i += findsecondgrouper(&expr[i], buf);
i++;
printf("%s\n", buf);
continue;
}
if (expr[i] == '(') {
i++;
i += parencase(&expr[i], buf);
i++;
printf("%s\n", buf);
continue;
}
}
// if not number, consider the current character and print its type   
   switch (expr[i]) { 
      case '+':
        printf("PLUS token\n");
        break;
      case '-':
        printf("MINUS token\n");
        break;
      case '*':
        printf("TIMES token\n");
        break;
      case '/':
        printf("DIVIDE token\n");
        break;
      case ';':
        printf(";\n");
        break;
      case '<':
        printf("<\n");
        break;
      case '>':
        printf(">\n");
        break;
      case '|':
        printf("|\n");
        break;
case '\'':
break;
      case '\"':
        printf("\"\n");

break;
case '(':
printf("(\n");
break;
case ')':
printf(")\n");
break;
       case ' ':
        // skip spaces
         break;
         default:
printf(""); 
        }
++i;
}

return 0;
}
                                             
