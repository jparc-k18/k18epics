#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

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
  int ch = 0;
  if( sscanf(rec->desc,"HTTP@%d",&ch) != 1 ) ch = 0;
  
  long nelem = 0;
  dbGetNelements(&rec->inp, &nelem);
  if(nelem>200) nelem=200;
  
  //printf("read ai\n");
  
  if(1<= ch &&  ch <= nelem){ 
    float buf[210];
    dbGetLink(&rec->inp, dbGetLinkDBFtype(&rec->inp), buf, NULL, &nelem);
    rec->val = buf[ch-1];
    //printf("%d %f\n",ch,buf[ch-1]);
  }else{
    printf("%s read_ai(): channel is out of range!\n",rec->name);
    return 0;
    //rec->val = -999999.999;
  }

  rec->udf = FALSE;
  return 2;
}

static long init_record(aiRecord *rec, int pass)
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

struct IOC devAiHTTP={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_ai,
  NULL
};
epicsExportAddress(dset,devAiHTTP);
