#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include<stdio.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int pid, *status[2], fd, pfd[2];
    char *argv1[25], *argv2[25], *file[25];
    argv1[0] = (char*)malloc(sizeof(char) * sizeof(argv[1]));
    argv2[0] = (char*)malloc(sizeof(char) * sizeof(argv[2]));
    file[0] = (char*)malloc(sizeof(char) * sizeof(argv[2]));
    file[1] = (char*)malloc(sizeof(char) * sizeof(argv[2]));

    printf("aa");
    printf("%s", argv[1]);
    printf("%s", argv[2]);

    pipe(pfd);
  if((pid = fork()) < 0) {
  printf("aaa");
  } else if (pid == 0) { 
    strncpy(argv1[0], argv[1], strlen(argv[1]));
    //argv[0][1] = "-a";
    argv1[1] = NULL;
    strncpy(file[0], argv[1], strlen(argv[1]));
    close(1);
    dup(pfd[1]);
    close(pfd[0]);
    close(pfd[1]);
  execvp(file[0], argv1);
  } 

  if((pid = fork()) < 0) {
    printf("bbb");
  } else if (pid == 0) { 
    strncpy(argv2[0], argv[2], strlen(argv[2]));
    //argv[1] = "-a";
    argv2[1] = NULL;
    strncpy(file[1], argv[2], strlen(argv[2]));
    close(0);
    dup(pfd[0]);
    close(pfd[0]);
    close(pfd[1]);
  execvp(file[1], argv2);
  } else {
  close(pfd[0]);
  close(pfd[1]);  
  wait(status[0]);
  wait(status[1]);

  }
  return 0;
}


