// -*- C++ -*-

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/resource.h>

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

#include <libxml/HTMLparser.h>

#include "UserSocket.hh"

namespace
{
  const int max_buf_len = 1024;
  const int nseconds = 10;
  UserSocket  *g_sock;
  std::string  g_host;
  int          g_port;
  bool OpenSocket( void )
  {
    if( g_sock )
      delete g_sock;
    g_sock = new UserSocket( g_host, g_port, 30 );
    if( g_sock ){
      g_sock->Print();
    }
    return ( g_sock && g_sock->IsOpen() );
  }
}

static long read_wf( waveformRecord *rec )
{
  static auto buf = new unsigned int[max_buf_len];
  const  auto nelm = rec->nelm;
  static auto prev_time = std::time(0);
  auto        curr_time = std::time(0);
  static std::vector<double> prev_val(nelm);
  std::vector<double>        curr_val(nelm);
  auto ptr = (float*)rec->bptr;
  if( g_sock && g_sock->IsOpen() ){
    if( g_sock->Read(buf, max_buf_len) <= 0 ){
      OpenSocket();
      return 1;
    }
    int data_start = max_buf_len + 1;
    for(int i=0; i<max_buf_len; ++i){
      if( buf[i] == 0xffff4ca1 ){
	data_start = i + 4; // header
      }
      if( i >= data_start && buf[i] != 0x0 ){
	unsigned int ch = i - data_start;
	if( ch < nelm ){
	  curr_val[ch] = buf[i] & 0xfffffff;
	}
      }
    }
  } else {
    OpenSocket();
    return 1;
  }

  if( curr_time - prev_time >= nseconds ){
    rec->nord = rec->nelm;
#if 0
    std::cout << curr_time <<  " devHulScaler0 " << __func__
	      << " " << rec->nord << "/" << rec->nelm << std::endl;
#endif
    for( int i=0, n=nelm; i<n; ++i ){
      ptr[i] = (curr_val[i] - prev_val[i])/nseconds;
      if( ptr[i] < 0 ) ptr[i] = 0;
      if( i > 0 && ptr[0] != 1. ){
	ptr[i] = -9999.;
      }
    }
    prev_time = curr_time;
    prev_val = curr_val;
    return 0;
  } else {
#if 0
    std::cout << curr_time <<  " devHulScaler1 " << __func__
	      << " " << rec->nord << "/" << rec->nelm << std::endl;
#endif
    return 1;
  }
}

static long init_record(waveformRecord *rec, int pass)
{
  std::cout << __func__ << std::endl;
  std::vector<std::string> elems;
  std::stringstream ss(rec->desc);
  std::string item;
  while( std::getline(ss, item, ' ') ){
    if( !item.empty() ){
      elems.push_back(item);
    }
  }
  if( elems.size() == 3 ){
    g_host = elems.at(1);
    g_port = std::strtoull(elems.at(2).c_str(), nullptr, 10);
    OpenSocket();
  }
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

struct IOC devHulScaler={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset,devHulScaler);
