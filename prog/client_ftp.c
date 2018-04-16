#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define DATASIZE 1000

int getargs(char *);

struct myftp_data {
  uint8_t type;
  uint8_t code;
  uint16_t length;
  char data[DATASIZE];
};

struct ftp_header {
  uint8_t type;
  uint8_t code;
  uint16_t length;
};

int main() {
  int sd, count, datalen, n, i, div;
  char *tp, cbuf[512], buf[1000], *com[10], *ip;
  uint8_t *ftp_pay;
  uint8_t *data, *data2;
  struct ftp_header *send_header, *recv_header;
  FILE *fp;
  uint32_t hostlong;
  uint16_t hostshort;
  in_port_t port;
  struct sockaddr_in myskt, skt;
  struct in_addr ipaddr;

  i = 0;
  div = 0;
  ip = (char *)malloc(sizeof(char) * 100);
  port = atoi("10000");
  strcpy(ip, "0.0.0.0");
  //tcp
  if((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&skt, 0, sizeof skt);
  skt.sin_family = AF_INET;
  skt.sin_port = htons(port);
  skt.sin_addr.s_addr = inet_addr(ip);

  if((count = connect(sd, (struct sockaddr *)&skt, sizeof skt)) < 0) {      //connect
    perror("connect");
    exit(1);
  }

  for( ; ; ) {                                                              //ftp開始
    printf("ftp$");

    for(i = 0; i < 2; i++) {
      com[i] = (char*)malloc(sizeof(char) * 20);
      div = getargs(com[i]);
      if(div == 1) break;
    }

    if(strcmp(com[0], "put") == 0) {                         
      strcpy(buf, com[1]);
      n = strlen(com[1]) + 1;
      buf[n - 1] = '\0';
      data = (uint8_t *)malloc(sizeof(struct ftp_header) + sizeof(char) * n);

      send_header = (struct ftp_header *)data;
      ftp_pay = data + sizeof(struct ftp_header);
      memcpy((char *)ftp_pay, buf, n);
      send_header->type = 0x06;
      send_header->code = 0x00;
      send_header->length = htons(n);

      if((count = send(sd, data, sizeof(struct ftp_header) + n, 0)) < 0) {
        perror("send");
        exit(1);
      }

      printf("aa\n");                                                      //ヘッダ受信
      recv(sd, data, sizeof(struct ftp_header), 0);
      recv_header = (struct ftp_header *)data;
      printf("%04x\n", recv_header->type);
      printf("%04x\n", recv_header->code);

      if(recv_header->type == 0x20) {
        if ((fp = fopen(com[1], "r")) == NULL) {                           //ファイル読み込み 
          fprintf(stderr, "%sのオープンに失敗しました.\n", com[1]);
          exit(EXIT_FAILURE);
        }

        for(;;) {
          n = fread(buf, sizeof(char), 1000, fp);

          if(n < 1000) {
            data2 = (uint8_t*)malloc(sizeof(struct ftp_header) + sizeof(uint8_t) * n);
            send_header = (struct ftp_header *)data2;
            ftp_pay = data2 + sizeof(struct ftp_header);
            memcpy((char *)ftp_pay, buf, n);
            send_header->type = 0x10;
            send_header->code = 0x00;
            send_header->length = htons(n);

            if((count = send(sd, data2, sizeof(struct ftp_header) + n, 0)) < 0) {
              perror("send");
              exit(1);
            }
            break;
          }
            data2 = (uint8_t*)malloc(sizeof(struct ftp_header) + sizeof(uint8_t) * n);
            send_header = (struct ftp_header *)data2;
            ftp_pay = data2 + sizeof(struct ftp_header);
            memcpy((char *)ftp_pay, buf, n);
            send_header->type = 0x10;
            send_header->code = 0x01;
            send_header->length = htons(n);
            
            if((count = send(sd, data2, sizeof(struct ftp_header) + n, 0)) < 0) {
              perror("send");
              exit(1);
            }
        }
        fclose(fp);
      }
    } else if(strcmp(com[0], "exit") == 0) {
      close(sd);
      exit(0);
    }
  }
  return 0;
}

int getargs(char *com) {
  char word[10], ch;
  int i;
  i = 0;
  while(1) {
    ch = getc(stdin);
    if(ch == ' ') {
      strcpy(com, word);
      return 0;
    } else if(ch == '\n') {
      strcpy(com, word);
      return 1;
    } else {
      word[i] = ch;
      i++;
    }
  }
}
