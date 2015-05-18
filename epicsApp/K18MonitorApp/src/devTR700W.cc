#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

#include <unistd.h>
#include <arpa/inet.h>

#include "alarm.h"
#include "cvtTable.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "link.h"
#include "waveformRecord.h"
#include "epicsExport.h"

void base64_decode(std::string const& s, unsigned char* ret);

static long read_wf(waveformRecord *rec)
{
  //connect socket
  char host[256];
  if( sscanf(rec->desc,"HTTP@%s",host) != 1 ){
    printf("TR700W no host ip address\n");
    return 0;
  }

  int port           = 80;
  double timeout_sec = 0.5;
  
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
    printf("%s | TR700W connection failure\n",date.c_str());
    close(sock);
    return 0;
  }
  
  // send HTTP request
  char cmdline[] = "GET /current.inc HTTP/1.0\r\nUser-Agent: Wget/1.12 (linux-gnu)\r\nAccept: */*\r\nHost: 192.168.30.31\r\nConnection: Keep-Alive\r\n\r\n ";
  write(sock, cmdline, strlen(cmdline));

  // receive
  std::string buf;
  int total_len=0;
  char c;
  while ( read(sock, &c, 1) > 0 ) {
    buf += c;
    total_len++;
    if(total_len > 65535) break;
  }
  close(sock);

  
  int pos1 = buf.find("C02", 0);
  std::string c02 = buf.substr(pos1+13, 10);
  //std::cout <<c02<<std::endl;

  int pos2 = buf.find("C03", 0);
  std::string c03 = buf.substr(pos2+13, 10);
  //std::cout <<c03<<std::endl;

  if(c02 != c03) return 0;
  
  int pos3 = buf.find("D20", 0);
  std::string d20 = buf.substr(pos3+14, 76);
  //std::cout <<d20<<std::endl;
  unsigned char ret[256];  
  base64_decode(d20, ret);
  

  //unsigned int utc = (ret[13]<<24) | (ret[12]<<16) | (ret[11]<<8) | ret[10];
  //printf("UTC: %u\n", utc);

  //unsigned int lct = (ret[17]<<24) | (ret[16]<<16) | (ret[15]<<8) | ret[14];
  //printf("LCT: %u\n", lct);

  unsigned short ch1 = (ret[27]<<8) | ret[26];
  //printf("ch1: %f\n", (ch1-1000)/10.);

  unsigned short ch2 = (ret[43]<<8) | ret[42];
  //printf("ch2: %f\n", (ch2-1000)/10.);
  
  
  float* ptr = (float*)rec->bptr;
  
  ptr[0] = (float)((ch1-1000)/10.);
  ptr[1] = (float)((ch2-1000)/10.);

  rec->nord = 2;
 
  return 0;
}

static long init_record(waveformRecord *rec, int pass)
{
  return 0;
}

struct IOC{
  long      number;
  DEVSUPFUN report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN read_wf;
  DEVSUPFUN special_linconv;
};

struct IOC devTR700W={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset,devTR700W);

char charconv(char c) {
  if (c >= 'A' && c <= 'Z') return (c - 'A');
  if (c >= 'a' && c <= 'z') return (c - 'a' + 26);
  if (c >= '0' && c <= '9') return (c - '0' + 52);
  if (c == '+') return 62;
  if (c == '/') return 63;
  return 0;
}

void base64_decode(std::string const& in, unsigned char* ret) {
  int len, i, iR;
  char a1, a2, a3, a4;
  len = in.size();
  i = 0;
  iR =0;
  
  while (1) {
    if (i >= len) break;
    a1 = charconv(in[i]);
    a2 = charconv(in[i+1]);
    a3 = charconv(in[i+2]);
    a4 = charconv(in[i+3]);
    ret[iR+0] = ((a1 << 2)&0xFF) | ((a2 >>4)&0xFF);
    ret[iR+1] = ((a2 << 4)&0xFF) | ((a3 >>2)&0xFF);
    ret[iR+2] = ((a3 << 6)&0xFF) | (a4&0xFF);
    i  += 4;
    iR += 3;
  }
} 
