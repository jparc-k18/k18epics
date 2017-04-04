#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  //connect socket
  char host[]        = "192.168.30.37";
  int port           = 62500;
  double timeout_sec = 2.0;

  struct timeval tv={(int)timeout_sec, (timeout_sec-(int)timeout_sec)*1000000.};
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt( sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv) );

  struct sockaddr_in addr;
  addr.sin_family       = AF_INET;
  addr.sin_port         = htons(port);
  addr.sin_addr.s_addr  = inet_addr(host);

  if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0){
    time_t t;
    t = time(NULL);
    std::string date(ctime(&t));
    date = date.substr(0, date.length()-1);
    printf("%s | connection failure\n",date.c_str());
    close(sock);
    return 0;
  }

  // receive login
  char buf[1024];
  read(sock, buf, 5);
  buf[5]='\0';
  printf("%s\n",buf);

  // send password
  char cmdline[200] = "password\r";
  write(sock, cmdline, 9);

  // receive OK
  read(sock, buf, 2);
  buf[2]='\0';
  printf("%s\n",buf);

  // send command
  int ptr=0;
  cmdline[ptr++]='T';
  cmdline[ptr++]='2';
  cmdline[ptr++]=0x0;
  cmdline[ptr++]=0x0;

  cmdline[ptr++]='E';
  cmdline[ptr++]='I';
  cmdline[ptr++]='R';
  cmdline[ptr++]='S';
  cmdline[ptr++]='P';
  cmdline[ptr++]=':';

  cmdline[ptr++]='M';
  cmdline[ptr++]='O';
  cmdline[ptr++]='D';
  cmdline[ptr++]='E';
  cmdline[ptr++]='=';
  cmdline[ptr++]=0x01;
  cmdline[ptr++]=0x00;
  cmdline[ptr++]=1;

  // cmdline[ptr++]='A';
  // cmdline[ptr++]='C';
  // cmdline[ptr++]='T';
  // cmdline[ptr++]='=';
  // cmdline[ptr++]=0x01;
  // cmdline[ptr++]=0x00;
  // cmdline[ptr++]=0;

  // cmdline[ptr++]='I';
  // cmdline[ptr++]='N';
  // cmdline[ptr++]='T';
  // cmdline[ptr++]='=';
  // cmdline[ptr++]=0x01;
  // cmdline[ptr++]=0x00;
  // cmdline[ptr++]=1;

  // cmdline[ptr++]='M';
  // cmdline[ptr++]='E';
  // cmdline[ptr++]='T';
  // cmdline[ptr++]='H';
  // cmdline[ptr++]='O';
  // cmdline[ptr++]='D';
  // cmdline[ptr++]='=';
  // cmdline[ptr++]=0x01;
  // cmdline[ptr++]=0x00;
  // cmdline[ptr++]=1;

  // cmdline[ptr++]='W';
  // cmdline[ptr++]='A';
  // cmdline[ptr++]='I';
  // cmdline[ptr++]='T';
  // cmdline[ptr++]='=';
  // cmdline[ptr++]=0x01;
  // cmdline[ptr++]=0x00;
  // cmdline[ptr++]=0;

  // cmdline[ptr++]='M';
  // cmdline[ptr++]='O';
  // cmdline[ptr++]='D';
  // cmdline[ptr++]='E';
  // cmdline[ptr++]='=';
  // cmdline[ptr++]=0x01;
  // cmdline[ptr++]=0x00;
  // cmdline[ptr++]=0;

  // cmdline[ptr++]='D';
  // cmdline[ptr++]='I';
  // cmdline[ptr++]='S';
  // cmdline[ptr++]='P';
  // cmdline[ptr++]='=';
  // cmdline[ptr++]=0x01;
  // cmdline[ptr++]=0x00;
  // cmdline[ptr++]=0;

  // cmdline[ptr++]='M';
  // cmdline[ptr++]='O';
  // cmdline[ptr++]='N';
  // cmdline[ptr++]='=';
  // cmdline[ptr++]=0x01;
  // cmdline[ptr++]=0x00;
  // cmdline[ptr++]=30;

  int cmd_num = ptr -4;
  printf("cmd_num: %04x\n",cmd_num);
  cmdline[2] = (char)(cmd_num&0xFF);
  cmdline[3] = (char)((cmd_num>>8)&0xFF);

  unsigned int sum = 0;
  for(int i=4;i<ptr;i++){
    sum += cmdline[i];
  }
  printf("check sum %04x\n",sum);

  cmdline[ptr++]=(char)(sum&0xFF);
  cmdline[ptr++]=(char)((sum>>8)&0xFF);

  write(sock, cmdline, ptr);

  // receive return
  for(int i=0;i<1024;i++){
    buf[i]='a';
  }

  int size = 400;
  read(sock, buf, size);
  //buf[size]='\0';
  for(int i=0;i<size-1;i++){
    if(i==2 || i==3) printf("%02x ", (unsigned char)buf[i]);
    else  printf("%c ",buf[i]);
  }

  printf("\n");

  close(sock);
  return 0;

  cmdline[0]='T';
  cmdline[1]='2';
  cmdline[2]=0x0e;
  cmdline[3]=0x00;
  cmdline[4]='E';
  cmdline[5]='C';
  cmdline[6]='R';
  cmdline[7]='N';
  cmdline[8]='T';
  cmdline[9]=':';
  cmdline[10]='M';
  cmdline[11]='O';
  cmdline[12]='D';
  cmdline[13]='E';
  cmdline[14]='=';
  cmdline[15]=0x01;
  cmdline[16]=0x00;
  cmdline[17]=0x09;
  cmdline[18]=0x22;
  cmdline[19]=0x03;

  sum = 0;
  for(int i=4;i<18;i++){
    sum += cmdline[i];
  }
  printf("%04x\n",sum);

  write(sock, cmdline, ptr);


  // receive return
  for(int i=0;i<256;i++){
    buf[i]='a';
  }

  size = 60;
  read(sock, buf, size);
  buf[size]='\0';
  for(int i=0;i<size-1;i++){
    if(i==2 || i==3 ||i==15 || i==16 ) printf("%02x ", (unsigned char)buf[i]);
    else  printf("%c ",buf[i]);
  }

  printf("\n");

  close(sock);
  return 0;
}
