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

  // Variables for printing the prompt
  char *user = getenv("USER");
  char *pwd = getenv("PWD");
  char hostname[1024];
  hostname[1023] = '\0';
  gethostname(hostname, 1023);
  // Used during fork:
  pid_t pid;
  // Main loop that reads a command and executes it
  while (1) {
    // prompt
    printf("%s@%s:%s>", user, hostname, pwd);

    // Variables for character reading
    char *input_string = malloc(100 * sizeof(char));
    char **separated = malloc(100 * sizeof(char *));
    int input_chari = 0;
    char input_charc = ' ';
    int count = 0;

    // Read input
    while(((input_chari = getchar()) != EOF) && (input_chari != 0) && (input_chari != '\n')){
      input_charc = (char)(input_chari);
      input_string[count] = input_charc;
      count++;
    }
    // Check to see if exit command has been input
    if(strcmp(input_string, "exit") == 0){
      free(input_string);
      free(separated);
      do_exit();
    }
    
    input_string[count++] = '\0';
    // Prints out to make sure that input_string is being filled properly.
    // printf("%s\n", input_string);

    // Runs separate function on the input, which should split the input into multiple strings, and get rid of all of the spaces.
    // execvp takes as arguments a char * (the command) and a char** (arguments) so we have to split it into a char**.
    separated = separate(input_string);
    
    // Ensures separated is returning the correct output
    printf("%s\n", separated[0]);




    // Child process created here. This needs to be modified, I copied it almost exactly from one of his lecture slides.
    pid = fork();
    // Check for fork error
    if (pid < 0){
      fprintf(stderr, "Fork Failed");
      return 1;
    }
    //Child process
    else if(pid == 0){
      execvp(separated[0], separated);
      printf("Child in Progress\n");
      exit(0);
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
    // If \ character is found, properly handle it (DON'T THINK WE NEED TO WORRY ABOUT THIS FOR THE MILESTONE, SO I COMMENTED IT OUT)
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
    // Add the temporary string to the args char **
    args[argcount] = temp;
  }
  free(temp);
  return args;
}

// Function which exits, printing the necessary message
//
void do_exit() {
  printf("So long and thanks for all the fish!\n");
  exit(0);
}
