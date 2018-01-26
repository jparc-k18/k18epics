#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

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
  char host[]        = "192.168.30.66";
  int port           = 8023;
  double timeout_sec = 2;
  struct timeval tv = { (int)timeout_sec,
			(timeout_sec-(int)timeout_sec)*1000000. };
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
    printf("%s | GL840(BGO) connection failure\n",date.c_str());
    close(sock);
    return 0;
  }

  //get channel number
  write(sock,":INFO:CH?\r\n",10);  // original /n
  char buf[255];
  read(sock,buf,255);
  int ch;
  sscanf(buf,":INFO:CH %d\r\n",&ch);  // original /r
  if(ch>100){
    printf("too many ch number: %d\n",ch);
    close(sock);
    return -1;
  }

  //get data
  write(sock,":MEAS:OUTP:ONE?\r\n",16);
  char header[8];
  read(sock,header,8);  //skip header

  float* ptr = (float*)rec->bptr;
  short data;
  for(int i=0;i<ch;i++){
    read(sock,&data,2);
    data = __bswap_16(data);

    if(data==0x7ffd) ptr[i] = -9999.;
    else ptr[i] = (float)data * 0.005;

    if( ptr[i] <= 0.01 )
      ptr[i] = -9999.;
  }

  close(sock);

  rec->nord = ch;
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

struct IOC devGL840_BGO={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset,devGL840_BGO);
