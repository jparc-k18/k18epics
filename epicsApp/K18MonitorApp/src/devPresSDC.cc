#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <signal.h>
std::stringstream oss;
std::stringstream ossvacuum;

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

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <curl/curl.h>
#include <vector>

#include "UserSocket.hh"
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output){
  size_t totalSize = size * nmemb;
  output->append(static_cast<char*>(contents), totalSize);
  return totalSize;
}

char dummy1[100];
char dummy2[100];
char dummy3[100];
char dummy4[100];
char dummy5[100];
char dummy6[100];

float p_adc0, p_adc1, p_adc2;
float p_vol0, p_vol1, p_vol2;
float p_pas0, p_pas1, p_pas2;


static long read_wf(waveformRecord *rec)
{
  std::cout << "read_wf" << std::endl;
  std::string outputString;
  CURL* curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.30.76/");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outputString);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "Failed to retrieve data from the URL: "
		<< curl_easy_strerror(res) << std::endl;
      curl_easy_cleanup(curl);
      return 1;
    }
    curl_easy_cleanup(curl);
    std::istringstream iss(outputString);
    std::string line;
    int c_num = 0;
    while (std::getline(iss, line)) {
      std::cout << line << std::endl;
      if(c_num==0){
	  iss>>dummy1>>p_adc0>>dummy2>>p_vol0>>dummy3>>p_pas0>>dummy4>>dummy5;
      }
      else if(c_num==1){
	iss>>dummy1>>p_adc1>>dummy2>>p_vol1>>dummy3>>p_pas1>>dummy4>>dummy5;
      }
      else if(c_num==2){
	iss>>dummy1>>p_adc2>>dummy2>>p_vol2>>dummy3>>p_pas2>>dummy4>>dummy5;
      }
      else {
	break;
      }
      c_num++;
      //      std::cout << line << std::endl;
    }
  }
  std::cout << "output is :" << outputString << std::endl;
  std::cout << " = ==== ==== === ==== " << std::endl;
  std::cout << "pressure is " << p_pas0 << std::endl;
  std::cout << "pressure is " << p_pas1 << std::endl;
  std::cout << "pressure is " << p_pas2 << std::endl;




  // fill
  float* ptr = (float*)rec->bptr;
  int ndata = 0;
  ptr[ndata++] = p_pas0;
  ptr[ndata++] = p_pas1;
  ptr[ndata++] = p_pas2;
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

struct IOC devPresSDC={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset, devPresSDC);
