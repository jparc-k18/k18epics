// -*- C++ -*-

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
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
  const int nseconds = 60;
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
#if 0
  static const std::string spark_txt("/data3/E42SubData/spark/spark.txt");
  static uint32_t last_time = 0;
  std::ifstream ifs(spark_txt);
  std::string line;
  while (ifs.good() &&
         std::getline(ifs, line) &&
         !line.empty() &&
         line[0] != '#') {
    std::istringstream iss(line);
    std::istream_iterator<std::string> begin(iss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> v(begin, end);
    if (v.size() < 2) continue;
    auto unix_time = std::stoull(v.at(0));
    auto spark_count = std::stoull(v.at(1));
    if (unix_time > last_time) {
    }
    std::cout << unix_time << " " << spark_count << std::endl;
  }
#endif

#if 1
  static auto buf = new unsigned int[max_buf_len];
  const  auto nelm = rec->nelm;
  static auto prev_time = std::time(0);
  auto        curr_time = std::time(0);
  tm t;
  localtime_r(&curr_time, &t);
  char tbuf[32];
  strftime(tbuf, 32, "%Y/%m/%d %H:%M:%S", &t);
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
    // std::ofstream ofs("/data3/E42SubData/tmp/spark.txt");
#if 0
    std::cout << curr_time <<  " devHulScaler " << __func__
	      << " " << rec->nord << "/" << rec->nelm << std::endl;
#endif
    for( int i=0, n=nelm; i<n; ++i ){
      ptr[i] = (curr_val[i] - prev_val[i]); // /nseconds;
#if 1
      std::ostringstream oss;
      oss << tbuf << " ch" << i << "  " << ptr[i] << std::endl;
      std::cout << oss.str();
      // ofs << oss.str();
#endif
      if( ptr[i] < 0 ) ptr[i] = 0;
      // if( i == 0 && std::abs(ptr[i] - nseconds) > 20 )
      if( i == 0 && ptr[i] == 0 )
        exit(1);
      // if( i > 0 && ptr[0] != nseconds ){
      //   ptr[i] = -9999.;
      // }
    }
    prev_time = curr_time;
    prev_val = curr_val;
    // if( std::abs(ptr[0] - nseconds) <= 20 ){
      rec->nord = rec->nelm;
      return 0;
    // }
  }

#if 0
  std::cout << curr_time <<  " devHulScaler1 " << __func__
	    << " " << rec->nord << "/" << rec->nelm << std::endl;
#endif
  rec->nord = 0;
  return 2;
#endif
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
