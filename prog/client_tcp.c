#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

int s, count, datalen;
char sbuf[512], *ip;
in_port_t port;
struct sockaddr_in myskt, skt;
struct in_addr ipaddr;

int main() {
  uint32_t hostlong;
  uint16_t hostshort;
  ip = (char *)malloc(sizeof(char) * 100);
  port = atoi("10000");
  strcpy(ip, "0.0.0.0");

  if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
 exit(1);   
  }

  strcpy(sbuf, "Hello!");
  datalen = strlen(sbuf);
  memset(&skt, 0, sizeof skt);
  skt.sin_family = AF_INET;
  skt.sin_port = htons(port);
  skt.sin_addr.s_addr = inet_addr(ip);

  if(connect(s, (struct sockaddr *)&skt, sizeof skt) < 0) {
    perror("connect");
    exit(1);
  }
    
  if((count = send(s, sbuf, datalen, 0)) < 0) {
    perror("send");
    exit(1);
  }

  if(close(s) < 0) {
    perror("close");
    exit(1);
  }
}
