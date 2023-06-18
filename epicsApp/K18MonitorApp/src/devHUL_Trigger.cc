// -*- C++ -*-

#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

#include <unistd.h>
#include <arpa/inet.h>

#include <UnpackerManager.hh>
#include <lexical_cast.hh>
#include <filesystem_util.hh>
#include <replace_string.hh>
#include <std_ostream.hh>

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
  auto ptr = reinterpret_cast<uint64_t*>(rec->bptr);

  static const std::string last_log("/misc/subdata/HUL_Trigger/last.log");
  std::ifstream ifs(last_log);
  std::string line;
  int ndata = 0;
  while(ifs.good() && std::getline(ifs, line)){
    if(line.empty()) continue;
    std::istringstream iss(line);
    std::istream_iterator<std::string> begin(iss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> v(begin, end);
    if (v.size() < 2) continue;
    uint64_t val = std::stoull(v.at(1));
    ptr[ndata++] = val;
    std::cout << "ch" << ndata << "\t" << v.at(0) << "\t"<< val << std::endl;
  }
  rec->nord = ndata;

  std::cout << "ndata = " << ndata << std::endl;
  return 0;
}

static long init_record(waveformRecord *rec)
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

struct IOC devHUL_Trigger={
		     6,
		     NULL,
		     NULL,
		     (DEVSUPFUN) init_record,
		     NULL,
		     (DEVSUPFUN) read_wf,
		     NULL
};
epicsExportAddress(dset,devHUL_Trigger);
