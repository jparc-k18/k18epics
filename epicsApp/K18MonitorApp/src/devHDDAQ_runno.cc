#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <vector>

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
#include "aiRecord.h"
#include "epicsExport.h"

static long read_ai(aiRecord *rec)
{
  FILE *pipe;
  char buf[256];

  std::string cat("ssh axis@");
  cat += std::getenv("HDDAQ_EBHOST");
  cat += " cat ";

  // Trig ON/OFF
  std::string command = cat + "daq/data/misc/trig.txt";
  pipe = ::popen(command.c_str(), "r");
  if( !pipe ){
    std::cerr << "#E popen() failed" << std::endl;
    return 0;
  }

  std::fgets( buf, sizeof(buf), pipe );
  ::pclose( pipe );

  std::string trig = buf;
  if( trig == "OFF" ){
    rec->val = 0;
    return 0;
  }

  // Run Number
  command = cat + "daq/data/misc/runno.txt";
  pipe = ::popen(command.c_str(), "r");
  if( !pipe ){
    std::cerr << "#E popen() failed" << std::endl;
    return 0;
  }

  std::fgets( buf, sizeof(buf), pipe );
  ::pclose( pipe );

  rec->val = std::atoi(buf);
  rec->udf = FALSE;

  return 2;
}

static long init_record(aiRecord *rec)
{
  return 0;
}

struct IOC{
  long      number;
  DEVSUPFUN report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN read_ai;
  DEVSUPFUN special_linconv;
};

struct IOC devHDDAQ_runno={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_ai,
  NULL
};
epicsExportAddress(dset,devHDDAQ_runno);
