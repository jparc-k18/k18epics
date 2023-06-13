#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
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

#include <typeinfo>
using namespace std;

static long read_wf(waveformRecord *rec)
{
  //connect socket
  char host[256];
  if( sscanf(rec->desc, "IP:%s", host) != 1 ){
    std::cout << "host: " << host << std::endl;
    printf("PMX18 no host ip address\n");
  }
  std::cout << "host: " << host << std::endl;
  int port = 5025;
  UserSocket sock( host, port);
  usleep(1000*1);
  if( !sock.IsOpen() ){
    return -1;
  }


  //get data
  char Command_Vread[100]="MEAS:VOLT?\n";
  char Command_Aread[100]="MEAS:CURR?\n";
  char Read_Vread[100];
  char Read_Aread[100];
  double Value_Vread=0;
  double Value_Aread=0;

  // volt
  sock.Write(Command_Vread,100);
  usleep(1000*10);
  sock.Read(Read_Vread,100);
  Value_Vread = strtod(Read_Vread,0);
  std::cout << "Vread: " << Read_Vread
	    << "       -> " << Value_Vread << " [V]" << std::endl;

  // current
  sock.Write(Command_Aread,100);
  usleep(1000*10);
  sock.Read(Read_Aread,100);
  Value_Aread = strtod(Read_Aread,0);
  std::cout << "Aread: " << Read_Aread
	    << "       -> " << Value_Aread << " [A]" << std::endl;
  float* ptr = (float*) rec->bptr;
  ptr[0] = Value_Vread;
  ptr[1] = Value_Aread;

  rec->nord = 2;

  sock.Close();

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

struct IOC devPMX18={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset, devPMX18);
