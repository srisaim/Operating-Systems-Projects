#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

//accept up to 16 command-line arguments
#define MAXARG 16

//allow up to 64 environment variables
#define MAXENV 64

//keep the last 500 commands in history
#define HISTSIZE 500

//accept up to 1024 bytes in one command
#define MAXLINE 1024

// Simply defining global array for environement variables and history.
char *envVariables[MAXENV];
int envCount = 0;
char history[HISTSIZE][MAXLINE];
int historyCount = 0;

static char **parseCmd(char cmdLine[]) {
  char **cmdArg, *ptr;
  int i;

  //(MAXARG + 1) because the list must be terminated by a NULL ptr
  cmdArg = (char **) malloc(sizeof(char *) * (MAXARG + 1));
  if (cmdArg == NULL) {
    perror("parseCmd: cmdArg is NULL");
    exit(1);
  }
  for (i = 0; i <= MAXARG; i++) //note the equality
    cmdArg[i] = NULL;
  i = 0;
  ptr = strsep(&cmdLine, " ");
  while (ptr != NULL) {
    // (strlen(ptr) + 1)
    cmdArg[i] = (char *) malloc(sizeof(char) * (strlen(ptr) + 1));
    if (cmdArg[i] == NULL) {
      perror("parseCmd: cmdArg[i] is NULL");
      exit(1);
    }
    strcpy(cmdArg[ i++ ], ptr);
    if (i == MAXARG)
      break;
    ptr = strsep(&cmdLine, " ");
  }
  return(cmdArg);
}

// Modified main function declaration to include "envp"
int main(int argc, char *argv[], char *envp[]) {
  char cmdLine[MAXLINE], **cmdArg;
  int status, i, debug;
  pid_t pid;

  // This for-loop is for copying environment variables from envp.
  for (envCount = 0; envp[envCount] != NULL && envCount < MAXENV; envCount++) {
	  envVariables[envCount] = strdup(envp[envCount]);
  }

  debug = 0;
  i = 1;
  while (i < argc) {
    if (! strcmp(argv[i], "-d") )
      debug = 1;
    i++;
  }

  while (( 1 )) {
    printf("bsh> ");                      //prompt
    fgets(cmdLine, MAXLINE, stdin);       //get a line from keyboard
    cmdLine[strlen(cmdLine) - 1] = '\0';  //strip '\n'

    // Store command in history
    strncpy(history[historyCount % HISTSIZE], cmdLine, MAXLINE);
    historyCount++;

    cmdArg = parseCmd(cmdLine);
    if (debug) {
      i = 0;
      while (cmdArg[i] != NULL) {
	printf("\t%d (%s)\n", i, cmdArg[i]);
	i++;
      }
    }

    //built-in command exit
    if (strcmp(cmdArg[0], "exit") == 0) {
      if (debug)
	printf("exiting\n");
      break;
    }
    //built-in command env to list all environment variables.
    else if (strcmp(cmdArg[0], "env") == 0) {
	    // We iterate through each environment variable, which is stored, and print it.
	    // The envVariables[j] contains 'VAR=value' strings.
	    for (int j = 0; j < envCount; j++) {
		    printf("%s\n", envVariables[j]);
	    }
    }
    //built-in command setenv to set or update an environment variable.
    else if (strcmp(cmdArg[0], "setenv") == 0) {
	    // Checking for correct number of arguments.
	    // Continue is to skip to next loop iteration if arguments are missing
	    if (cmdArg[1] == NULL || cmdArg[2] == NULL) {
		    fprintf(stderr, "Usage: setenv VAR VALUE\n");
		    continue;
	    }

	    // Preparing variable name for comparison (appending '=').
	    char *varNameWithEqual = malloc(strlen(cmdArg[1]) + 2); // 1 for '=', 1 for '\0'
	    sprintf(varNameWithEqual, "%s=", cmdArg[1]);
	    int varIndex = -1;

	    // Searching for the variable in existing environment variables.
	    for (int j = 0; j < envCount; j++) {
		    if (strncmp(envVariables[j], varNameWithEqual, strlen(varNameWithEqual)) == 0) {
			    varIndex = j;
			    break;
		    }
	    }

	    // Just to free the temporary memory.
	    free(varNameWithEqual);

	    // Creating new variable or update existing one.
	    // Allocating memory.
	    // Formatting as 'VAR=value'.
	    char *newVar = (char *)malloc(strlen(cmdArg[1]) + strlen(cmdArg[2]) + 2);
	    sprintf(newVar, "%s=%s", cmdArg[1], cmdArg[2]);

	    // Updating existing variable.
	    // Freeing old value and assigning new value.
	    if (varIndex != -1) {
		    free(envVariables[varIndex]);
		    envVariables[varIndex] = newVar;
	    } 
	    // Adding new variable and storing it.
	    else if (envCount < MAXENV) {
		    envVariables[envCount++] = newVar;
	    }
	    // Environment variable limit has reached and freeing allocated memory. 
	    else {
		    fprintf(stderr, "Environment variable limit reached.\n");
		    free(newVar);
	    }
    }
    //built-in command unsetenv to remove an environment variable.
    else if (strcmp(cmdArg[0], "unsetenv") == 0) {

	    // Checking if the variable name is provided.
	    // Continue is to skip next iteration if variable name is  missing.
	    if (cmdArg[1] == NULL) {
		    fprintf(stderr, "Usage: unsetenv VAR\n");
		    continue;
	    }

	    // Preparing the variable name for comparison (appending '=').
	    // Allocating memory.
	    // Index of variable in the array and -1 if it isn't found.
	    char *varNameWithEqual = malloc(strlen(cmdArg[1]) + 2); // 1 for '=', 1 for '\0'
	    sprintf(varNameWithEqual, "%s=", cmdArg[1]);
	    int varIndex = -1;

	    // Searching for the variable in the environment variables list.
	    for (int j = 0; j < envCount; j++) {
		    
		    if (strncmp(envVariables[j], varNameWithEqual, strlen(varNameWithEqual)) == 0) {
			    varIndex = j;
			    break;
		    }
	    }

	    // Freeing temporary memory.
	    free(varNameWithEqual);

	    // Remove the variable if found.
	    // Free memory allocated for the variable
	    if (varIndex != -1) {
		    free(envVariables[varIndex]);

		    // Shift remaining variables down in the array
		    for (int j = varIndex; j < envCount - 1; j++) {
			    envVariables[j] = envVariables[j + 1];
		    }

		    // Decrement count of environment variables
		    envCount--;
	    } 
	    else {
		    fprintf(stderr, "Variable not found: %s\n", cmdArg[1]);
	    }
    }
    //built-in command cd to change the current working directory.
    else if (strcmp(cmdArg[0], "cd") == 0) {

	    char *targetDir;

	    // If no directory is specified or argument is empty, use the HOME directory
	    if (cmdArg[1] == NULL || strcmp(cmdArg[1], "") == 0) {
		    for (int j = 0; j < envCount; j++) {

			    // Simply getting path from "HOME=/path".
			    if (strncmp(envVariables[j], "HOME=", 5) == 0) {
				    targetDir = envVariables[j] + 5; // Skip past "HOME="
				    break;
			    }
		    }
	    } 
	    else {
		    // Using specified directory.
		    targetDir = cmdArg[1];
	    }

	    // Attemots to change the directory
	    // Prints an error message if directory change is failed.
	    if (chdir(targetDir) != 0) {
		    perror("chdir");
		    continue;
	    }
	    
	    // Update the PWD (Present Working Directory) environment variable.
	    char cwd[PATH_MAX];
	    
	    if (getcwd(cwd, sizeof(cwd)) != NULL) {

		    // Find and update the PWD variable in the environment array.
		     for (int j = 0; j < envCount; j++) {

			     // Free the old PWD memory and allocate new memory for updated PWD.
			     if (strncmp(envVariables[j], "PWD=", 4) == 0) {
				     free(envVariables[j]); // To prevent memory leak.
				     envVariables[j] = malloc(strlen(cwd) + 5);

				     if (envVariables[j] == NULL) {
					     perror("malloc");
					     exit(1); // Handles memory allocation error.
				     }

				     // Set new PWD value.
				     sprintf(envVariables[j], "PWD=%s", cwd);
				     break;
			     }
		     }
	     }
	     
	     // Handles error in getting current directory.
	     else {
		     perror("getcwd");
	     }
    }
    // Creating a case for the "pwd" command in order to test the "cd" command, which prints current working directory.
    else if (strcmp(cmdArg[0], "pwd") == 0) {

	    // Buffer to store the current working directory.
	    char cwd[PATH_MAX];

	    // Retrieve and print the current working directory.
	    if (getcwd(cwd, sizeof(cwd)) != NULL) {
		    printf("%s\n", cwd);
	    }
	    // If not successful, error message is printed. 
	    else {
		    perror("getcwd");
	    }
    }
    // Creating a case for the "ls" command in order to test the "cd" command, which lists directory contents.
    else if (strcmp(cmdArg[0], "ls") == 0) {
	    // Simply uses the 'ls' command of the underlying system.
	    system("ls");
    }
    //built-in command history to display the list of previously entered commands.
    else if (strcmp(cmdArg[0], "history") == 0) {

	    // Calculates start index.
	    int start = historyCount - HISTSIZE < 0 ? 0 : historyCount - HISTSIZE;

	    // Looping through and printing each command from history.
	    // Also prints command number and the command.
	    for (int i = start; i < historyCount; i++) {
		    printf("%5d  %s\n", i + 1, history[i % HISTSIZE]);
	    }
    }

    //implement how to execute Minix commands here

    //the following is a template for using fork() and execv()
    //***remove this else case from your final code for bsh.c
    else {
      if (debug)
	printf("calling fork()\n");
      pid = fork();
      if (pid != 0) {
	if (debug)
	  printf("parent %d waiting for child %d\n", getpid(), pid);
	waitpid(pid, &status, 0);
      }
      else {
	status = execv(cmdArg[0], cmdArg);
	if (status) {
	  printf("\tno such command (%s)\n", cmdArg[0]);
	  return 1;
	}
      }
    }
    //***remove up to this point

    //clean up before running the next command
    i = 0;
    while (cmdArg[i] != NULL)
      free( cmdArg[i++] );
    free(cmdArg);
  }

  return 0;
}
