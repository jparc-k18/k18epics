#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <byteswap.h>

int main()
{

  int sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  
  addr.sin_family       = AF_INET;
  addr.sin_port         = htons(1111);
  addr.sin_addr.s_addr  = inet_addr("192.168.30.30");

  int ret = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
  if(ret<0){
    printf("ZR-RX40 connection failure\n");
    return 0;
  }
  
  write(sock,":INFO:CH?\n",10);
  char buf[255];
  read(sock,buf,255);
  //printf("%s\n",buf);
  int ch;
  //printf("%x aa\n",aaa[11]);
  sscanf(buf,":INFO:CH %d\r",&ch);
  printf("CH: %d\n",ch);

  write(sock,":MEAS:OUTP:ONE?\n",16);
  
  char header[8];
  read(sock,header,8);
  //buf[8]='\0';

  //printf("%s\n",header);

  //int rec;
  //sscanf(header,"#6%d",&rec);
  //printf("REC: %d\n",rec);

  // char buf1[7];
  // strncpy(buf1,buf+2,6);
  // buf1[6]='\0';
  
  // printf("%i\n",atoi(buf1));
  
  unsigned short int data;

  for(int i=0;i<ch;i++){
    read(sock,&data,2);
    printf("%d %04x\n",i+1,__bswap_16(data));
  }

  close(sock);

  return 0;
}
