#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <vector>

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

#include "UserSocket.hh"

static long read_wf(waveformRecord *rec)
{
  // connect socket w/ proxy server
  UserSocket sock( "192.168.30.1", 8080, 0.5 );

  // connect socket w/o proxy server
  // UserSocket sock( "192.153.109.17", 80, 0.5 );

  if( !sock.IsOpen() )
    return -1;

  // send HTTP request
  char cmdline[] = "GET http://www-cont.j-parc.jp/HD/index HTTP/1.0\n\n";
  sock.Write( cmdline, strlen(cmdline) );

  // receive
  std::string buf;
  int total_len=0;
  char c;
  while ( sock.Read( &c, 1 ) > 0 ) {
    buf += c;
    total_len++;
    if(total_len > 65535) break;
  }

  // analize table
  typedef std::vector<std::string> tb_row;
  std::vector<tb_row> tb_content;

  int index1=0;
  while(1){
    int pos1 = buf.find("<tr>", index1);
    if(pos1 == (int)std::string::npos) break;
    int pos2 = buf.find("</tr>", index1);
    if(pos2 == (int)std::string::npos) break;

    index1 = pos2 + 5;

    std::string sub1 = buf.substr(pos1+4, pos2-pos1-5);
    //printf("%s\n",sub1.c_str());

    tb_row row;
    int index2=0;
    while(1){
      int pos3 = sub1.find(">", index2);
      if(pos3 == (int)std::string::npos) break;
      int pos4 = sub1.find("</td>", index2);
      if(pos4 == (int)std::string::npos) break;

      index2 = pos4 + 5;

      std::string sub2 = sub1.substr(pos3+1, pos4-pos3-1);
      row.push_back(sub2);
      //printf("%s\n",sub2.c_str());
    }
    tb_content.push_back(row);
  }

  // fill
  float* ptr = (float*)rec->bptr;
  int ndata=0;
  int column_num = tb_content.size();
  for(int i=0;i<column_num;i++){

    float val=0;
    char tmp[255];

    int ret = sscanf(tb_content[i][1].c_str(),"%f %s",&val,tmp);

    if(ret==2){
      ptr[ndata++]=val;
    }else if(ret==1){
      ptr[ndata++]=val;
    }else{
      ptr[ndata++]=(float)-9999;
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

struct IOC devHTTP_accel={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset,devHTTP_accel);
