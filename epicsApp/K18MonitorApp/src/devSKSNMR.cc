// -*- C++ -*-

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

static long read_ai(aiRecord *rec)
{
  //connect socket
  UserSocket sock( "192.168.30.20", 10001 );
  if( !sock.IsOpen() )
    return -1;

  //get value
  sock.Write("n",1);

  char buf[64];
  sock.Read(buf,64);
  //printf("SKS Field: %f [T]\n",atof(buf)*1E-6);

  rec->val = atof(buf)*1E-6;
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

struct IOC devSKSNMR={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_ai,
  NULL
};
epicsExportAddress(dset,devSKSNMR);
