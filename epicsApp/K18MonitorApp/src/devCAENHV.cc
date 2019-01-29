#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

#include "CAENHVWrapper.h"

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
  char board[16];
  char host[256];
  int slot;
  int ch_max;
  if( sscanf(rec->desc,"%s %s %d %d",board, host, &slot, &ch_max) != 4 ){
    printf("CAEN HV: DESC format error\n");
    return 0;
  }

  if(ch_max>24) ch_max=24;

  CAENHV_SYSTEM_TYPE_t sysType;
  int link = LINKTYPE_TCPIP;
  char UserName[] = "admin";
  char Passwd[] = "admin";
  static int sysHndl=-1;

  if(!strcmp(board,"SY1527")) sysType = SY1527;
  else if(!strcmp(board,"SY2527")) sysType = SY2527;
  else{
    printf("CAEN HV: unknown borad type %s\n",board);
    return 0;
  }

  if(sysHndl==-1){
    int ret = CAENHV_InitSystem(sysType, link, host, UserName, Passwd, &sysHndl);
    if(ret != CAENHV_OK){
      printf("\nCAENHV_InitSystem: %s (num. %d)\n\n", CAENHV_GetError(sysHndl), ret);
      return 0;
    }
  }

  unsigned short ChList[24] = {
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23
  };
  float V0Set[24]     = {};
  float I0Set[24]     = {};
  float VMon[24]      = {};
  float IMon[24]      = {};
  unsigned int Pw[24] = {};

  CAENHV_GetChParam(sysHndl, slot, "V0Set", ch_max, ChList, V0Set);
  CAENHV_GetChParam(sysHndl, slot, "I0Set", ch_max, ChList, I0Set);
  CAENHV_GetChParam(sysHndl, slot, "VMon", ch_max, ChList, VMon);
  CAENHV_GetChParam(sysHndl, slot, "IMon", ch_max, ChList, IMon);
  CAENHV_GetChParam(sysHndl, slot, "Pw", ch_max, ChList, Pw);

  // {
  //   int ret = CAENHV_DeinitSystem(sysHndl);
  //   if( ret != CAENHV_OK ){
  //     printf("CAENHV_DeinitSystem: %s (num. %d)\n\n",
  // 	     CAENHV_GetError(sysHndl), ret);
  //     return 0;
  //   }
  // }

  float* ptr = (float*)rec->bptr;
  int ndata=0;

  for(int i=0;i<ch_max;i++){
    // printf("CH:%d  V0Set:%f  I0Set:%f  VMon:%f  IMon:%f  Pw:%s\n",
    // 	   i, V0Set[i], I0Set[i], VMon[i], IMon[i], Pw[i]? "ON":"OFF");
    ptr[ndata++] = V0Set[i];
    ptr[ndata++] = I0Set[i];
    ptr[ndata++] = VMon[i];
    ptr[ndata++] = IMon[i];
    ptr[ndata++] = (float)Pw[i];
  }

  rec->nord = ndata;

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

struct IOC devCAENHV={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset,devCAENHV);
