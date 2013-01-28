/**
 * CS3600, Spring 2013
 * Project 1 Starter Code
 * (c) 2013 Alan Mislove
 *
 * You should use this (very simple) starter code as a basis for 
 * building your shell.  Please see the project handout for more
 * details.
 */

#include "3600sh.h"
#include <unistd.h>
#include <stdio.h>

#define USE(x) (x) = (x)
char **separate(char *input);
char **parse(char *input);
int main(int argc, char*argv[]) {
  // Code which sets stdout to be unbuffered
  // This is necessary for testing; do not change these lines
  USE(argc);
  USE(argv);
  setvbuf(stdout, NULL, _IONBF, 0); 

  char *user = getenv("USER");
  char *pwd = getenv("PWD");
  char hostname[1024];
  hostname[1023] = '\0';
  gethostname(hostname, 1023);
  pid_t pid;
  // Main loop that reads a command and executes it
  while (1) {
    // You should issue the prompt here
    printf("%s@%s:%s>", user, hostname, pwd);

    // You should read in the command and execute it here
    char *input_string = malloc(100 * sizeof(char));
    char **separated = malloc(100 * sizeof(char *));
    char **arguments = malloc(100 * sizeof(char *));
    int input_chari = 0;
    char input_charc = ' ';
    int count = 0;

    while(((input_chari = getchar()) != EOF) && (input_chari != 0) && (input_chari != '\n')){
      input_charc = (char)(input_chari);
      input_string[count] = input_charc;
      count++;
    }

    if(strcmp(input_string, "exit") == 0){
      free(input_string);
      free(separated);
      free(arguments);
      do_exit();
    }
    
    input_string[count++] = '\0';
    //input_string = *p_input_string;
    printf("%s\n", input_string);
    separated = separate(input_string);
    
    for(int i = 0; i < 1; i++){
      printf("%s\n", separated[i]);
    }
    pid = fork();
    // Check for fork error
    if (pid < 0){
      fprintf(stderr, "Fork Failed");
      return 1;
    }
    //Child process
    else if(pid == 0){
      execvp(separated[0], arguments);
      printf("Child in Progress\n");
    }
    // Parent process
    else{
      wait(NULL);
      printf("Child Complete\n");
    }
    

  }
  
  return 0;
}

// Function that separates the input string into multiple words
char **separate(char *input){
  char **args = (char **) malloc(100 * sizeof(char *));
  char *temp = (char *) malloc(100 * sizeof(char));
  int argcount = 0;
  int tempcount = 0;
  for(int i = 0; i < ((int)strlen(input)); i++){

  // If space, skip
    if(input[i] == ' '){
      args[argcount] = temp;
      free(temp);
      temp = (char *) malloc(100 * sizeof(char));
      argcount++;
      tempcount = 0;
    }
    // If \ character is found, properly handle it
/**    else if(input[i] == '\\'){
      char d = getchar();
      if(d == 't'){
        temp[tempcount] = '	';
        tempcount++;
      }
    }**/
    // If normal character, add to temporary string  
    else{
      temp[tempcount] = input[i];
      tempcount++;
    }

    args[argcount] = temp;
  }
  free(temp);
  return args;
}

// Function that parses the input string into an array of char *s
char **parse(char *input){
  int index = 0;
  char *newArg = (char *) malloc(100 * sizeof(char)); 
  int argIndex = 0;
  char **argArray = (char **) malloc(100 * sizeof(char *));
  int arrayIndex = 0;
  while(input[index] != '\0'){
    if(input[index] == ' '){
      if((input[index+1] != ' ') || (input[index+1])){
        argArray[arrayIndex] = newArg;
        arrayIndex++;
        free(newArg);
        newArg = (char *) malloc(100 * sizeof(char));
      }
    }
    index++;
    argIndex++;
  }
  return argArray;
}

// Function which exits, printing the necessary message
//
void do_exit() {
  printf("So long and thanks for all the fish!\n");
  exit(0);
}
