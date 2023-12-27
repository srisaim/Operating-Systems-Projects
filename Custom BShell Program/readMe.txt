Project 3 Writeup
Name: Sri Sai Manjunadh Valluru
Student ID: 01852662
Username: srisaim

For Project 3, I'll briefly summarize what I was able to achieve for sections 3.1-3.3, which mainly focused on the implementation of specific built-in commands in my bsh shell (env, setenv, unsetenv, cd, history). I'll also discuss the special features I added (pwd, ls), which were useful for further testing of my code. So, the main tasks I was able to accomplish were successfully implementing the following commands: env, setenv, unsetenv, cd, and history.

For the env command:
It simply lists all the current environment variables, and when testing this command, it successfully displayed the variables, which mirrored how the standard env command functions in Unix-like shells.

For the setenv command:
It simply allows the user to set or update an environment variable. I was also able to implement this command with error handling cases if in any scenario insufficient arguments were provided.

For the unsetenv command:
It simply removes an environment variable from the shell's environment. My implementation successfully worked, as it also included proper memory management in order to avoid leaks.

For the cd command:
It simply handles the task of moving to specific different directories from the current working directory. This includes navigating to the home directory when no argument is provided, like "cd." It also updates the pwd environment variable accordingly.

For the history command:
It simply lists the last 500 commands entered by the user in chronological order of when each command was inputted. It successfully displays a list of commands that have been executed as well as its corresponding sequence numbers. This function successfully mirrors the history command found in standard Unix-like shells.

Testing for each command input is found below, and for further information on how each method was implemented, refer to the comments found in bsh.c.

Testing:
make bsh
make: 'bsh' is up to date.

*** To List ***
./bsh
bsh> env
SHELL=/bin/bash
KRB5CCNAME=FILE:/tmp/krb5cc_12915_m7aYPb
PWD=/home/srisaim/cs444/proj3
LOGNAME=srisaim
... (etc.)

*** To Set/Add ***
bsh> setenv MY_VAR HelloWorld
bsh> env
SHELL=/bin/bash
KRB5CCNAME=FILE:/tmp/krb5cc_12915_m7aYPb
PWD=/home/srisaim/cs444/proj3
LOGNAME=srisaim
... (etc.)
MY_VAR=HelloWorld

*** To Remove ***
bsh> unsetenv MY_VAR
bsh> env
SHELL=/bin/bash
KRB5CCNAME=FILE:/tmp/krb5cc_12915_m7aYPb
PWD=/home/srisaim/cs444/proj3
LOGNAME=srisaim
... (etc.)
(Removed)

*** To View History ***
bsh> history
     1  env
     2  setenv MY_VAR HelloWorld
     3  env
     4  unsetenv MY_VAR
     5  env
     6  history

*** To Change Directories ***
bsh> pwd
/courses/cs444/f23/hdeblois/srisaim/proj3
bsh> cd
bsh> pwd
/home/srisaim
bsh> cd /tmp
bsh> pwd
/tmp
bsh> cd
bsh> pwd
/home/srisaim
bsh> ls
a.out  cs110  cs310  cs341  cs410  cs444  cs446  hello.c  scp
bsh> cd cs444
bsh> pwd
/courses/cs444/f23/hdeblois/srisaim
bsh> ls
hw1.pdf  hw2.pdf  hw3.pdf  proj1  proj2  proj3

*** To Exit ***
bsh> exit

For some of the unfinished tasks:
I didn't truly implement accurate error handling and edge cases. Even though env, setenv, unsetenv, cd, and history are functioning correctly, I believe I could've included thorough testing for edge cases and error scenarios. For example, error cases for handling invalid paths in cd, attempting to unset non-existing environment variables, etc. When also looking at the memory management, I believe I could've tested more thoroughly for memory leaks, especially in commands like setenv and unsetenv that involve dynamic memory allocation, because it's crucial to ensure the shell is functioning robustly.

Quick overview of Special Features in my code:
ls command:
This simply lists the contents of directories, which is particularly useful for testing the cd command. 

pwd command:
This simply prints the current working directory, which is useful for testing the cd command's functionality.

 
