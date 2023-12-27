#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

// this code creates the pipe: "cat pipeDemo.c | wc"
int main(int argc, char *argv[]) {
  pid_t pid1, pid2;
  int status, fd[2];
  char *parameters[3];

  pipe(fd);
  pid2 = fork();
  if (pid2 == 0) {
    //I am the child process for the 2nd part of the pipeline
    close(fd[1]);          //close the output of the pipe
    close(STDIN_FILENO);   //close stdin
    dup(fd[0]);            //dup the input of the pipe to stdin
    close(fd[0]);          //close the input of the pipe
    parameters[0] = (char *) malloc(sizeof(char) * 16);
    strcpy(parameters[0], "/usr/bin/wc");
    parameters[1] = NULL;
    execv(parameters[0], parameters);
  }
  else{
    pid1 = fork();
    if (pid1 == 0){
      //I am the child process for the 1st part of the pipeline
      close(fd[0]);         //close the input of the pipe
      close(STDOUT_FILENO); //close stdout
      dup(fd[1]);           //dup the output of the pipe to stdout
      close(fd[1]);         //close the output of the pipe
      parameters[0] = (char *) malloc(sizeof(char) * 16);
      strcpy(parameters[0], "/bin/cat");
      parameters[1] = (char *) malloc(sizeof(char) * 16);
      strcpy(parameters[1], "pipeDemo.c");
      parameters[2] = NULL;
      execv(parameters[0], parameters);
    }
    else{
      //I am the parent process, bsh
      close(fd[0]);
      close(fd[1]);
      waitpid(pid1, &status, 0);
    }
    waitpid(pid2, &status, 0);
  }
  return 0;
}
