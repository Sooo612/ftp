#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#define TKN_NORMAL 0
#define TKN_PIPE 1
#define TKN_REDIR_IN 2
#define TKN_REDIR_OUT 3
#define TKN_ENT 4
#define TKN_WORD_ENT 5


int gettoken(char *, int);

int main(int argc, char *argv[]) {
  char *word, ch, *token[100], *file[100], *cmd1[25], *cmd2[25];
  int len, i, tmp, token_num[100], *status[2], pfd[2], pid;

  for( ; ; ){
    printf("$");
    printf("%d", *token_num);
    //トークン取得
    for(i = 0; ; ) {
      token[i] = (char*)malloc(sizeof(char) * 100);
      tmp = gettoken(token[i], 10);
      if(tmp == TKN_WORD_ENT) {                                     //改行を含む
        token_num[i] = TKN_NORMAL;
        i++;
        token[i] = (char*)malloc(sizeof(char) * 100);
        strncpy(token[i], "\n\0", 2);
        token_num[i] = TKN_ENT;
        break;
      } else if(tmp == TKN_ENT) {
        token_num[i] = TKN_ENT;
        break;
      } else {
        token_num[i] = tmp;
        i++;
      }
    }

/*    for( ; i >= 0; i--){
      printf("%d\n", token_num[i]);
    }
*/


                                                                  //トークン実行
    for(i = 0; ; ) {
      //printf("aa\n");
      if(token_num[i + 1] == TKN_PIPE) {                            //パイプ処理
        pipe(pfd);
        cmd1[0] = (char*)malloc(sizeof(char) * sizeof(token[i]));
        cmd2[0] = (char*)malloc(sizeof(char) * sizeof(token[i + 2]));
        file[0] = (char*)malloc(sizeof(char) * sizeof(token[i + 2]));
        file[1] = (char*)malloc(sizeof(char) * sizeof(token[i + 2]));
  
  
        //プロセス１
        if((pid = fork()) < 0 ) {
        } else if(pid == 0) {
          strncpy(cmd1[0], token[i], strlen(token[i]));
          //argv[0][1] = "-a";
          cmd1[1] = NULL;
          strncpy(file[0], token[i], strlen(token[i]));
          close(1);
          dup(pfd[1]);
          close(pfd[0]);
          close(pfd[1]);
          execvp(file[0], cmd1);
        }

        //プロセス２
        if((pid = fork()) < 0) {
          //printf("bbb");
        } else if (pid == 0) {
          strncpy(cmd2[0], token[i + 2], strlen(token[i + 2]));
          //argv[1] = "-a";
          cmd2[1] = NULL;
          strncpy(file[1], token[i + 2], strlen(token[i + 2]));
          close(0);
          dup(pfd[0]);
          close(pfd[0]);
          close(pfd[1]);
          execvp(file[1], cmd2);
        } else {
          close(pfd[0]);
          close(pfd[1]);
          wait(status[0]);
          wait(status[1]);
          i += 2;
        }
      } else if(token_num[i + 1] == TKN_ENT) {                                //改行処理
        cmd1[0] = (char*)malloc(sizeof(char) * sizeof(token[i]));
        file[0] = (char*)malloc(sizeof(char) * sizeof(token[i]));
        if((pid =fork()) < 0) {

        } else if (pid == 0) {
          strncpy(cmd1[0], token[i], strlen(token[i]));
          //argv[1] = "-a";
          cmd1[1] = NULL;
          strncpy(file[0], token[i], strlen(token[i]));
          execvp(file[0], cmd1);
          printf("dd");
          i++;
          continue;
        }
      } else if(token_num[i] == TKN_ENT) {
        printf("\n");
        token_num[i] = -1;
        break;
      } else {
        break;      
      }
      break;
    }
    puts("");
  }
  return 0;
}


int gettoken(char *token, int len) {
  int i, j = 0;
  char ch, word[100];
  for(i = 0 ; ; ) {
    ch = getc(stdin);
    if(isalnum(ch)) {
      word[i] = ch;
      i++;
      continue;
    } else if(ch == '\n'){
      if(i == 0) {
      word[i] = '\n';
      word[i + 1] = '\0';
      strncpy(token, word, strlen(word));
      return TKN_ENT;
      } else {
        word[i] = '\0';
        strncpy(token, word, strlen(word));
        return TKN_WORD_ENT;
      }
    } else if(ch == ' ' | ch == '\t'){
      continue;
    }else if(ch == '|'){
      if(i != 0){
        word[i] = '\0';
        strncpy(token, word, strlen(word));
        ungetc(ch, stdin);
        return TKN_NORMAL;
      }else{
        word[0] = ch;
        word[1] = '\0';
        strncpy(token, word, strlen(word));
        return TKN_PIPE;
      }
    }else if(ch == '>'){
      if(i != 0){
        word[i] = '\0';
        strncpy(token, word, strlen(word));
        ungetc(ch, stdin);
        return TKN_NORMAL;
      }else{
        word[0] = ch;
        word[1] = '\0';
        strncpy(token, word, strlen(word));
        return TKN_REDIR_IN;
      }
    }else if(ch == '<'){
      if(i != 0){
        word[i] = '\0';
        strncpy(token, word, strlen(word));
        ungetc(ch, stdin);
        return TKN_NORMAL;
      }else{
        word[0] = ch;
        word[1] = '\0';
        strncpy(token, word, strlen(word));
        return TKN_REDIR_OUT;
      }
    }
  }
  return 0;
}
