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

namespace
{
using namespace hddaq::unpacker;
UnpackerManager& gUnpacker = GUnpacker::get_instance();
}

static long read_wf(waveformRecord *rec)
{
  auto ptr = reinterpret_cast<uint64_t*>(rec->bptr);

  ++gUnpacker;
  if(gUnpacker.eof())
    std::exit(0);

  ptr[0] = gUnpacker.get_run_number();
  ptr[1] = gUnpacker.get_event_number();
  {
    static const auto id = gUnpacker.get_device_id("UnixTime");
    if (gUnpacker.get_entries(id, 0, 0, 0, 0) > 0 &&
	gUnpacker.get_entries(id, 0, 0, 0, 1) > 0) {
      auto lo = gUnpacker.get(id, 0, 0, 0, 0);
      auto hi = gUnpacker.get(id, 0, 0, 0, 1);
      ptr[2] = lo | (hi << 28);
    }
  }
  {
    static const auto id = gUnpacker.get_device_id("Scaler");
    if (gUnpacker.get_entries(id, 0, 0, 0, 0) > 0) {
      ptr[3] = gUnpacker.get(id, 0, 0, 0, 0);
    }
  }

  rec->nord = 4;

  return 0;
}

static long init_record(waveformRecord *rec)
{
  std::string conf_file = "/misc/software/param/conf/analyzer.conf";
  std::string conf_dir = hddaq::dirname(conf_file);
  std::ifstream conf(conf_file);
  std::map<std::string, std::string> key_map;
  while (conf.good()) {
    std::string l;
    std::getline(conf, l);
    std::istringstream iss(l);
    std::istream_iterator<std::string> iss_begin(iss);
    std::istream_iterator<std::string> iss_end;
    std::vector<std::string> param(iss_begin, iss_end);
    if (param.empty() || param[0].empty())
      continue;
    if (param.size() == 2) {
      std::string key   = param[0];
      std::string value = param[1];
      hddaq::replace_all(key, ":","");
      hddaq::replace_all(key, ";","");
      hddaq::replace_all(key, " ","");
      try {
	std::stof(value);
      } catch (const std::exception& what) {
	if (value[0] != '/'){
	  value = hddaq::realpath(std::string(conf_dir+"/"+value));
	}
      }
      hddaq::cout << " key = "   << std::setw(12) << std::left << key
                  << " value = " << value << std::endl;
      key_map[key] = value;
    }
  }
  gUnpacker.set_config_file(key_map["UNPACK"],
			    key_map["DIGIT"],
			    key_map["CMAP"]);

  {
    std::istringstream iss(rec->desc);
    std::istream_iterator<std::string> iss_begin(iss);
    std::istream_iterator<std::string> iss_end;
    std::vector<std::string> desc(iss_begin, iss_end);
    gUnpacker.set_istream(desc.back());
  }

  gUnpacker.initialize();
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

struct IOC devHDDAQ={
		     6,
		     NULL,
		     NULL,
		     (DEVSUPFUN) init_record,
		     NULL,
		     (DEVSUPFUN) read_wf,
		     NULL
};
epicsExportAddress(dset,devHDDAQ);
