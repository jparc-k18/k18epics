#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <ctime>
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

  static std::string summary_txt("/data3/E40SubData/epics_2018jun/mppcbias_summary.txt");
  std::ifstream ifs( summary_txt.c_str() );
  if( !ifs.is_open() ){
    std::cerr << "ERROR: cannot open file: " << summary_txt << std::endl;
    rec->nord = 0;
    return 0;
  }

  std::cout << std::time(0) << std::endl;

  std::string line;
  int ndata = 0;
  while( !ifs.eof() && std::getline(ifs, line) ){
    std::cout << line << std::endl;
    std::stringstream ss(line);
    std::string label;
    std::string state;
    ss >> label >> state;
    if( label.find("192.168.") == 0 ){
      if( state.find("OFF") == 0 )
	ptr[ndata++] = 0.;
      else if( state.find("ON") == 0 )
	ptr[ndata++] = 10.;
      else
	ptr[ndata++] = -10.;
    }
    if( label.find("CH") == 0 ){
      std::string date;
      std::string time;
      double vset = -9999.;
      double vmon = -9999.;
      double imon = -9999.;
      double temp = -9999.;
      ss >> date >> time >> vset >> vmon >> imon >> temp;
      if( state.find("OFF") == 0 )
	ptr[ndata++] = 0.;
      else if( state.find("ON") == 0 )
	ptr[ndata++] = 10.;
      else if( state.find("NOUPDATE") == 0 )
	ptr[ndata++] = 20.;
      else
	ptr[ndata++] = -10.;
      ptr[ndata++] = vset * 10.;
      ptr[ndata++] = vmon * 10.;
      ptr[ndata++] = imon * 10.;
      ptr[ndata++] = temp * 10.;
    }
  }

  int i = 0;
  for( int b=0; b<2; ++b ){
    std::cout << i << " " << ptr[i++] << std::endl;
    for( int j=0; j<8; ++j ){
      for( int j=0; j<5; ++j ){
	std::cout << ptr[i++] << " ";
      }
      std::cout << std::endl;
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
