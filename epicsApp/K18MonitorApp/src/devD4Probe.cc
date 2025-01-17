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
#include "aiRecord.h"
#include "epicsExport.h"

#include "UserSocket.hh"

namespace
{
  UserSocket* sock = nullptr;
}

static long read_ai(aiRecord *rec)
{
  if (!sock->IsOpen())
    std::exit(1);

  //get value
  if(sock->Write("f",1) < 0)
    std::exit(1);

  char buf[12];
  sock->Read(buf,12);
  printf("D4 Field: %f [T]\n",atof(buf));

  rec->val = atof(buf);
  rec->udf = FALSE;

  return 2;
}

static long init_record(aiRecord *rec)
{
  sock = new UserSocket("192.168.30.21", 1111);
  if (!sock || !sock->IsOpen())
    std::exit(1);
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

struct IOC devD4Probe={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_ai,
  NULL
};
epicsExportAddress(dset,devD4Probe);
