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
void exe_str(int, uint8_t *, int);

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

  recv(sd, data, sizeof(struct ftp_header), 0);

  recv_header = (struct ftp_header *)data;
  datalen = ntohs(recv_header->length);
  type = recv_header->type;
  printf("%04x\n", recv_header->type);
  code = recv_header->code;
  printf("%04x\n", recv_header->code);
  ftp_pay = data + sizeof(struct ftp_header);
  printf("%s\n", ftp_pay);
  switch(type) {
    case 0x05 : exe_retv();
                break;
    case 0x06 : exe_str(sd, ftp_pay, datalen);
                break;
    default:    break;
  }
}

void exe_str(int sd, uint8_t *ftp_pay, int n) {
  int count, pid;
  FILE *fp;
  char *buf;
  struct ftp_header *send_header, *recv_header;
  uint8_t *data;

  printf("aa\n");
  if((pid = fork()) < 0) {
  } else if(pid == 0){

   // *buf = (char *)ftp_pay;
    memcpy(buf, ftp_pay, n);
    printf("%s\n", buf);
    if((fp = fopen(buf, "w")) == NULL) {
      fprintf(stderr, "%sのオープンに失敗しました．\n", buf);
      exit(EXIT_FAILURE);
    }
  }

  send_header->type = 0x10;
  send_header->code = 0x02;

  data = (uint8_t *)malloc(sizeof(struct ftp_header) * sizeof(send_header));

  if((count = send(sd, data, sizeof(struct ftp_header), 0)) < 0) {
    perror("send");
    exit(1);
  }

  recv(sd, data, sizeof(struct ftp_header), 0);
  recv_header = (struct ftp_header *)data;

  if(recv_header->type == 0x20) {
    if((pid = fork()) < 0) {
    } else if(pid == 0) {
      //書きこみ
    }
  }
}

void exe_retv() {
}
