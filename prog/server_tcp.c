#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>


int main() {
  int s, s2, count;
  char buf[512];
  in_port_t port;
  struct sockaddr_in skt, myskt;
  struct in_addr ipaddr;
  uint32_t hostlong;
  uint16_t hostshort;
  socklen_t sktlen;
  port = atoi("10000");

  if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&myskt, 0, sizeof(myskt));
  myskt.sin_family = AF_INET;
  myskt.sin_port = htons(port);
  myskt.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
    perror("bind");
    exit(1);
  }

  if(listen(s, 5) < 0) {
    perror("listen");
    exit(1);
  }

  sktlen = sizeof(skt);

  if((s2 = accept(s, (struct sockaddr *)&skt, &sktlen)) < 0) {
    perror("accept");
    exit(1);
  }

  if((count = recv(s2, buf, sizeof(buf), 0)) < 0) {
    perror("recv");
    exit(1);
  }
  
  printf("%s", buf);
  puts("");
  if(close(s) < 0) {
    perror("close");
    exit(1);
  }
}
