#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int s, s2, count, sktlen;
char buf[512];
in_port_t port;
struct sockaddr_in skt, myskt;
struct in_addr ipaddr;

int main() {                                                                //socket
  uint32_t hostlong;
  uint16_t hostshort;
  socklen_t sktlen;
  port = atoi("10000");
  if((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  sktlen = strlen(buf);
  memset(&myskt, 0, sizeof myskt);
  myskt.sin_family = AF_INET;
  myskt.sin_port = htons(port);
  myskt.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
    perror("bind");
    exit(1);
  }

  if((count = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&skt, &sktlen)) < 0) {
    perror("recvfrom");
    exit(1);
  }
  fprintf(stdout, "%s", buf);
  puts("");  
  if(close(s) < 0) {
    perror("close buf");
  }
}


