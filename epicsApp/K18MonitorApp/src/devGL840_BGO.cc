
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <string>

#include <unistd.h>
#include <sys/resource.h>

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

#include "UserSocket.hh"

static long read_wf(waveformRecord *rec)
{
  //connect socket
  UserSocket sock( "192.168.30.66", 8023 );
  if( !sock.IsOpen() )
    return -1;

  //get channel number
  sock.Write(":INFO:CH?\r\n",10);  // original /n
  char buf[255];
  sock.Read(buf,255);
  int ch;
  std::sscanf(buf,":INFO:CH %d\r\n",&ch);  // original /r
  if(ch>100){
    printf("too many ch number: %d\n",ch);
    return -1;
  }

  //get data
  sock.Write(":MEAS:OUTP:ONE?\r\n",16);
  char header[8];
  sock.Read(header,8);  //skip header

  float* ptr = (float*)rec->bptr;
  short data;
  for(int i=0;i<ch;i++){
    sock.Read(&data,2);
    data = __bswap_16(data);

    if(data==0x7ffd) ptr[i] = -9999.;
    else ptr[i] = (float)data;

    if( ptr[i] <= 0.01 )
      ptr[i] = -9999.;
  }

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
