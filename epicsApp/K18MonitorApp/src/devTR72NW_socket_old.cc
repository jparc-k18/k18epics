#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
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
  //connect socket
  char host[256]; // ip address
  // std::string host ="";
  //  char SerialNumber[4]; // Serial Number
  //  char SerialNumberStr[256];
  //  char SerNum[4]; // Serial Number
  if( sscanf(rec->desc,"HTTP@%s",host) != 1 ){
    printf("TR72NW_socket no host ip address\n");
  }

  int port = 57172;
  UserSocket sock( host, port );
  if( !sock.IsOpen() )
    return -1;

  // std::string filepath = "/home/sks/k18epics/debug_socket.txt";
  // std::ofstream ofs(filepath);
  // ofs << "---- debug 1 ----" << std::endl;
  // ofs << "ip address: " << host << std::endl;
  // ofs.close();

  //get data
  //  char SerialNumber[4]={0x53, 0x14, 0x18, 0x52}; // for 30.38
  // char SerialNumber[4]={static_cast<char>(0xA9), 0x6B, 0x18, 0x52}; // for 30.36
  char SerialNumber[4]={static_cast<char>(0xB2), 0x6B, 0x18, 0x52}; // for 30.35
  //  char SerialNumber[4]; // Serial Number
  //  char words[] = "";
  //  words  = rec->desc;




  if (sscanf(rec->desc, "%*[^:]: %hhx %hhx %hhx %hhx",
	     &SerialNumber[0], &SerialNumber[1], &SerialNumber[2], &SerialNumber[3]) != 1)
    {
      printf("Cannot get TR72NR serial number\n");
      //    return 0;
    }

  //debug
  // open file
  //  std::string filename = "debug.txt";
  //  std::string homedir = std::getenv("HOME");
  //  std::string filepath = homedir + "/" + filename;
  // sscanf(rec->desc, "SerialNumber@%s %s %s %s", &SerNumStr[0], &SerNumStr[1], &SerNumStr[2], &SerNumStr[3]);
  // for (int i = 0; i < 4; i++) {
  //   SerNum[i] = strtol(SerNumStr[i], NULL, 16);
  // }
  char ReadCommand[13]={0x0B, 0x00,
                        0x01, 0x33, 0x00, 0x04, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x38, 0x00};
  //std::cout << std::endl << " * Write command * " << std::endl;
  for(int n=0;n<4;n++){
    //std::cout << std::hex << std::setfill('0') << std::setw(2) << +SerialNumber[n] << std::dec << std::endl;
  }
  for(int n=0;n<13;n++){
    //std::cout << std::hex << std::setfill('0') << std::setw(2) << +ReadCommand[n] << std::dec << std::endl;
  }
  sock.Write(SerialNumber, 4);
  sock.Write(ReadCommand, 13);

  usleep(1000*50);


  //std::cout << std::endl << " * Read date * " << std::endl;
  int index=0;
  short data;

  // header
  sock.Read(&data,1);
  //std::cout << index << " " << std::hex << std::setfill('0') << std::setw(2) << data << std::dec << std::endl;
  index++;
  sock.Read(&data,1);
  //std::cout << index << " " << std::hex << std::setfill('0') << std::setw(2) << data << std::dec << std::endl;
  index++;
  sock.Read(&data,1);
  //std::cout << index << " " << std::hex << std::setfill('0') << std::setw(2) << data << std::dec << std::endl;
  index++;

  // data size
  sock.Read(&data,2);
  //std::cout << "data size: " << data << std::endl;

  // temp
  sock.Read(&data,2);
  std::string sub1;
  sub1 = std::to_string( (data-1000.)/10. );
  std::cout << "temp raw: " << data << " temp: " << (data-1000.)/10. << std::endl;

  // hum
  sock.Read(&data,2);
  std::string sub2;
  sub2 = std::to_string( (data-1000.)/10. );
  std::cout << "hum raw: " << data << " hum: " << (data-1000.)/10. << std::endl;

  float* ptr = (float*)rec->bptr;

  ptr[0] = (float)( atof(sub1.c_str()) );
  ptr[1] = (float)( atof(sub2.c_str()) );

  rec->nord = 2;

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

struct IOC devTR72NW_socket={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset,devTR72NW_socket);
