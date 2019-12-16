#include <ctime>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

#include <unistd.h>
#include <arpa/inet.h>

#include <curl/curl.h>
#include <curl/easy.h>

#include "picojson.h"

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

#include "UserSocket.hh"

namespace
{
  typedef picojson::array  pjarray;
  typedef picojson::object pjobject;
  typedef picojson::value  pjvalue;

  CURL *curl = NULL;
  curl_slist *headers = NULL;
  std::string response_data;
  //___________________________________________________________________________
  // stream is pointer to set CURLOPT_WRITEDATA
  size_t
  on_receive( char* ptr, size_t size, size_t nmemb, void* stream )
  {
    const size_t sizes = size * nmemb;
    ((std::string*)stream)->append( (char*)ptr, sizes );
    return sizes;
  }
}

static long read_wf( waveformRecord *rec )
{
  if( !curl ){
    curl_easy_cleanup( curl );
    return 1;
  }

  {
    response_data.clear();
    std::string().swap( response_data );
  }

  CURLcode res = curl_easy_perform( curl );
  if(res != CURLE_OK){
    curl_easy_cleanup(curl);
    return 1;
  }

  // curl_easy_cleanup( curl );

  float* ptr = (float*)rec->bptr;

  pjvalue val;
  const std::string err = picojson::parse( val, response_data );
  if( !err.empty() ){
    std::cerr << err << std::endl;
    return 2;
  }

  pjobject& top_obj = val.get<pjobject>();

  pjvalue& err_val = top_obj["error"];
  if( !err_val.is<picojson::null>() ){
    pjobject& obj = err_val.get<pjobject>();
    double code = obj["code"].get<double>();
    std::string message = obj["message"].get<std::string>();
    std::cerr << code << " " << message << std::endl;
    return 2;
  }

  pjarray& array = top_obj["devices"].get<pjarray>();
  pjarray::iterator itr, end=array.end();
  int nch = 0;
  for(itr=array.begin(); itr!=end; ++itr){
    pjobject& obj = (*itr).get<pjobject>();
    pjarray& ch = obj["channel"].get<pjarray>();
    double v1 = std::strtod( ch[0].get<pjobject>()["value"]
			     .get<std::string>().c_str(),
			     NULL );
    double v2 = std::strtod( ch[1].get<pjobject>()["value"]
			     .get<std::string>().c_str(),
			     NULL );
    ptr[nch++] = v1*10.;
    ptr[nch++] = v2*10.;

#if 0
    std::time_t unixtime = std::strtoul( obj["unixtime"]
					 .get<std::string>().c_str(),
					 NULL, 0 );
    std::cout << v << std::endl;
    std::cout << std::time(0) << std::endl;
    std::cout << unixtime << std::endl;
    tm* tm = localtime(&unixtime);
    std::cout << tm->tm_year+1900 << "/"
	      << tm->tm_mon+1 << "/"
	      << tm->tm_mday << " "
	      << tm->tm_hour << ":"
	      << tm->tm_min << ":"
	      << tm->tm_sec << std::endl;
#endif
  }

  rec->nord = 2;

  return 0;
}

static long init_record(waveformRecord *rec, int pass)
{
  const std::string& url("https://api.webstorage.jp/v1/devices/current");
  const std::string& apikey("3s9hlc1852d78oqmb3i0nb25fsmtgn41558ourcstckp7");
  // account for viewing, not administrator
  const std::string& loginid("rbac0104");
  const std::string& loginpass("beamtime");
  static const std::string& post_data("{\"api-key\": \"" + apikey
				      + "\", \"login-id\": \"" + loginid
				      + "\", \"login-pass\": \"" + loginpass
				      + "\"}");
  curl = curl_easy_init();
  headers = curl_slist_append( headers, "Expect:" );
  headers = curl_slist_append( headers, "Host: api.webstorage.jp:443" );
  headers = curl_slist_append( headers, "Content-Type: application/json" );
  headers = curl_slist_append( headers, "X-HTTP-Method-Override: GET" );
  // curl_easy_setopt( curl, CURLOPT_VERBOSE, 1 );
  // curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 1 );
  curl_easy_setopt( curl, CURLOPT_URL, url.c_str() );
  curl_easy_setopt( curl, CURLOPT_HTTPHEADER, headers );
  curl_easy_setopt( curl, CURLOPT_POST, 1 );
  curl_easy_setopt( curl, CURLOPT_POSTFIELDS, post_data.c_str() );
  curl_easy_setopt( curl, CURLOPT_POSTFIELDSIZE, post_data.size() );
  curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, on_receive );
  curl_easy_setopt( curl, CURLOPT_WRITEDATA, &response_data );
  curl_easy_setopt( curl, CURLOPT_PROXY, "http://k18server:8080" );

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

struct IOC devTR72NW={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset,devTR72NW);
