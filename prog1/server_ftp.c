#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define DATASIZE 1000

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

void excute_com(int);
void getargs(char *, char *);
void exe_retv();
void exe_str(int, int);

int main() {
  int master_s, client_s, count, datalen, pid, n;
  char buf[512];
  in_port_t port;
  struct sockaddr_in skt, myskt;
  struct in_addr ipaddr;
  struct ftp_header send_header, recv_header;
  uint32_t hostlong;
  uint16_t hostshort;
  uint8_t *ftp_pay, *data;
  socklen_t sktlen;
  port = atoi("10000");

  if((master_s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&myskt, 0, sizeof(myskt));
  myskt.sin_family = AF_INET;
  myskt.sin_port = htons(port);
  myskt.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(master_s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
    perror("bind");
    exit(1);
  }

  if(listen(master_s, 5) < 0) {
    perror("listen");
    exit(1);
  }

  sktlen = sizeof(skt);

  for( ; ; ) {
    if((client_s = accept(master_s, (struct sockaddr *)&skt, &sktlen)) < 0) {
      perror("accept");
      exit(1);
    }

    if((pid = fork()) < 0) {
    } else if(pid == 0) {
      excute_com(client_s);
    } else {
      close(client_s);
    }
  }
}

void excute_com(int sd) {
  int datalen;
  char *buf;
  uint8_t type, code, *data, *ftp_pay;
  struct ftp_header *recv_header;

  for(;;) {
    recv(sd, data, sizeof(struct ftp_header), 0);

    recv_header = (struct ftp_header *)data;
    datalen = ntohs(recv_header->length);
    printf("%d\n", datalen);
    type = recv_header->type;
    printf("%04x\n", recv_header->type);
    code = recv_header->code;
    printf("%04x\n", recv_header->code);
    //ftp_pay = data + sizeof(struct ftp_header);

    switch(type) {
      case 0x05: 
        exe_retv();
        break;
      case 0x06:
        printf("0x06 received!\n");
        exe_str(sd, datalen);
        break;
      default:
        break; //exit(0);
    }
  }
}


void exe_str(int sd, int n) {
  int count, pid;
  FILE *fp;
  char buf[1000];
  struct ftp_header *send_header, recv_header;
  uint8_t *pay, *send_data;
  //recv pay
  pay = (uint8_t *)malloc(n);
  if((count = recv(sd, pay, n, 0)) < 0) {
    perror("recv\n");
    exit(1);
  }
  //edit file
  memcpy(buf, (char *)pay, n);
  if ((fp = fopen(buf, "w")) == NULL) {
    fprintf(stderr, "%sのオープンに失敗しました.\n", buf);
    exit(EXIT_FAILURE);
  }
  //printf("%s\n", buf);

  send_data = (uint8_t *)malloc(sizeof(struct ftp_header));                //send header
  send_header = (struct ftp_header *)send_data;
  send_header->type = 0x20;
  send_header->code = 0x02;
  send_header->length = 0;

  if((count = send(sd, send_data, n, 0)) < 0) {
    perror("recv\n");
    exit(1);
  }
  free(send_data);
  free(pay);

  //recv_header = (struct ftp_header *)malloc(sizeof(struct ftp_header));    //recv header

  for(;;) {
    printf("a");
    if((count = recv(sd, &recv_header, sizeof(struct ftp_header), 0)) < 0) {
      perror("recv\n");
      exit(1);
    }
    printf("a");

    n = ntohs(recv_header.length);
    pay = (uint8_t *)malloc(sizeof(char) * n);
    while(n > 0) {
      if((count = recv(sd, pay, n, 0)) < 0) {                                //recv pay
        perror("recv\n");
        exit(1);
      }
      printf("a\n");
      fwrite((char *)pay, sizeof(char), count, fp);
      n -= count;
    }
    free(pay);
    if(recv_header.code == 0x00) {
      break; 
    }
  }
  fclose(fp);
}

void exe_retv() {
}
