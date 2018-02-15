#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <string>
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

#include "UserSocket.hh"

static long read_wf(waveformRecord *rec)
{
  //UDP socket
  UserSocket sock( 12345 );
  if( !sock.IsOpen() )
    return -1;

  char buf[2048]={0};

  if( sock.Recv( buf, sizeof(buf), 0 ) < 0){
    std::time_t t;
    t = std::time(NULL);
    std::string date( std::ctime(&t) );
    date = date.substr(0, date.length()-1);
    //printf("%s | HBJ UDP recv timeout\n",date.c_str());
    return 0;
  }

  //printf("%s\n",buf);

  int ndata=0;
  float* ptr = (float*)rec->bptr;

  std::stringstream ss(buf);
  std::string elem;

  while(ss >> elem){
    double val;
    //printf("%s\n",elem.c_str());
    if( sscanf(elem.c_str(),"%lf",&val) == 1 ){
      ptr[ndata] = (float)val;
      //printf("HBJ_LABVIEW: val=%f\n",ptr[ndata]);
      ndata++;
    }
  }

  rec->nord = ndata;
  //printf("HBJ_LABVIEW: ndata=%d\n",ndata);
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

struct IOC devHBJ={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset,devHBJ);
