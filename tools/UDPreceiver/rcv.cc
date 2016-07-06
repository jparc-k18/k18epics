#include <stdio.h>

#include <unistd.h>
#include <arpa/inet.h>


int main()
{
  double timeout_sec = 5;
  struct timeval tv={(int)timeout_sec, (timeout_sec-(int)timeout_sec)*1000000.};
  
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv) );

  struct sockaddr_in addr;
  addr.sin_family       = AF_INET;
  addr.sin_port         = htons(12345);
  addr.sin_addr.s_addr  = INADDR_ANY;

  bind(sock, (struct sockaddr *)&addr, sizeof(addr));

  char buf[2048]={0};
  
  int ret = recv(sock, buf, sizeof(buf), 0);

  printf("%d %s\n",ret, buf);

  close(sock);

  return 0;
}
