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


static long read_wf(waveformRecord *rec)
{
  //connect socket
  char host[256];
  if( sscanf(rec->desc,"HTTP@%s",host) != 1 ){
    printf("TR700W no host ip address\n");
    return 0;
  }

  int port           = 80;
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
    printf("%s | TR700W connection failure\n",date.c_str());
    close(sock);
    return 0;
  }

  // send HTTP request
  char cmdline[] = "GET /cdata.inc HTTP/1.0\r\nUser-Agent: Wget/1.12 (linux-gnu)\r\nAccept: */*\r\nHost: 192.168.30.31\r\nConnection: Keep-Alive\r\n\r\n ";
  write(sock, cmdline, strlen(cmdline));

  // receive
  std::string buf;
  int total_len=0;
  char c;
  while ( read(sock, &c, 1) > 0 ) {
    if(0x20 < c && c <0x7f){
      buf += c;
      total_len++;
    }
    if(total_len > 65535) break;
  }
  close(sock);

  if(total_len!=271) {
    time_t t;
    t = time(NULL);
    std::string date(ctime(&t));
    date = date.substr(0, date.length()-1);
    printf("%s | TR700W comunication error\n",date.c_str());
    return 0;
  }

  std::string sub1 =  buf.substr(125, 4);
  //printf("sub1: %s\n",sub1.c_str());

  std::string sub2 =  buf.substr(155, 2);
  //printf("sub2: %s\n",sub2.c_str());


  float* ptr = (float*)rec->bptr;

  ptr[0] = (float)( atof(sub1.c_str()) );
  ptr[1] = (float)( atof(sub2.c_str()) );

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

