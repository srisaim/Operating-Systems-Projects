#include <stdio.h>
#include <stdlib.h>

//env var are sent in as the third parameter
int main(int argc, char *argv[], char *envp[]){
  int k;

  k = 0;
  while (envp[k] != NULL) {
    printf("%s\n", envp[k]);
    k++;
  }
  printf("\n%d env variables\n", k);
  return 0;
}
