#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
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
  float* ptr = (float*)rec->bptr;

  std::ifstream ifs("/data3/E40SubData/epics_2018jun/mppcbias_summary.txt");
  if( !ifs.is_open() ){
    rec->nord = 0;
    return 0;
  }

  std::string line;
  int ndata = 0;
  while( !ifs.eof() && std::getline(ifs, line) ){
    std::stringstream ss(line);
    std::string label;
    std::string state;
    ss >> label >> state;
    if( label.find("192.168.") == 0 ){
      if( state.find("ON") == 0 )
	ptr[ndata++] = 1;
      else if( state.find("OFF") == 0 )
	ptr[ndata++] = 0;
      else
	ptr[ndata++] = -1;
    }
    if( label.find("CH") == 0 ){
      std::string date;
      std::string time;
      double vset;
      double vmon;
      double imon;
      double temp;
      ss >> date >> time >> vset >> vmon >> imon >> temp;
      ptr[ndata++] = vset * 10.;
      ptr[ndata++] = vmon * 10.;
      ptr[ndata++] = imon * 10.;
      ptr[ndata++] = temp * 10.;
    }
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

struct IOC devMppcBias={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset,devMppcBias);
