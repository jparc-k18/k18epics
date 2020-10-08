#include <ctime>
#include <iomanip>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
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

#include "UserSocket.hh"
#include "Utility.hh"

namespace
{
  std::string host;
  int         addr;

namespace mqv
{
  const std::string Stx( "\x02" );
  const std::string Etx( "\x03" );
  const std::string Term( "\r\n" );
  const std::string SubAddr( "00" );
  const std::string DeviceCode( "X" );

  //___________________________________________________________________________
  void CplSum( const std::string& msg, int len, std::string& buf )
  {
    unsigned char* str = (unsigned char*)msg.c_str();
    int num = 0;
    for( unsigned char* ch=&str[0]; ch<&str[len]; ++ch ){
      num += *ch;
    }
    num = ( ( - ( num & 0x000000FF ) ) & 0x000000FF );
    const int CplSumSize = 2;
    char cbuf[CplSumSize];
    sprintf( cbuf, "%02X", num );
    buf = cbuf;
  }

  //_____________________________________________________________________________
  bool Apply( UserSocket& sock,
	      const std::string& command, std::string& receive )
  {
#ifdef DEBUG
    std::cout << "Command : " << command << std::endl;
#endif
    receive.clear();
    std::stringstream header;
    header << Stx << std::setw(2) << std::setfill('0') << addr
	   << SubAddr << DeviceCode;
    std::string message = header.str() + command + Etx;
    std::string sum;
    CplSum( message, message.size(), sum );
    message += sum + Term;
    sock.Write( message.c_str(), message.size() );
    std::string buf;
    unsigned char c;
    while( sock.Read( &c, 1 ) > 0 ){
      buf += c;
      if( buf.find( Term ) != std::string::npos )
	break;
      if( buf.empty() )
      	return false;
    }
    unsigned long cplsumpos = buf.size() - sum.size() - Term.size();
    CplSum( buf, cplsumpos, sum );
    if( ( buf[cplsumpos] != sum[0] ) || ( buf[cplsumpos+1] != sum[1] ) ){
      printf( "CheckSum : %x <-> %x\n", buf[cplsumpos], sum[0] );
      printf( "CheckSum : %x <-> %x\n", buf[cplsumpos+1], sum[1] );
      return false;
    }
    receive = buf.substr( header.str().size(),
			  cplsumpos - header.str().size() - Etx.size() );
#ifdef DEBUG
    std::cout << "Receive : " << receive << std::endl;
#endif
    int ret = std::strtol( receive.substr(1, 2).c_str(), NULL, 10 );
    switch( ret ){
    case 0:
      return true;
    case 21: case 23:
      std::cerr << "#Waring Something is wrong : " << ret << std::endl;
      return false;
    // case 40: case 41: case 43: case 46: case 47: case 48: case 99:
    //   std::cerr << "#ERROR Something is wrong : " << ret << std::endl;
    //   return false;
    default:
      std::cerr << "#ERROR Something is wrong : " << ret << std::endl;
      return false;
    }
  }
} // namespace mqv

} // namespace mqv

static long read_wf( waveformRecord *rec )
{
  float* ptr = (float*)rec->bptr;
  for( int i=0; i<5; ++i ){
    ptr[i] = -9999;
  }
  rec->nord = 5;
  UserSocket sock( host, 4001 );
  if( !sock.IsOpen() )
    return 1;

  std::string command;
  std::string receive;
  std::vector<std::string> data;
  std::vector<int>         val;
  command = "RS,1201W,8";
  if( mqv::Apply( sock, command, receive ) ){
    data = utility::split( receive, ',' );
    val.resize( data.size() );
    for(int i=0, n=data.size(); i<n; ++i ){
      val[i] = std::strtol( data[i].c_str(), NULL, 10 );
    }
    ptr[0] = (float)( ( val[3]<<2 ) | val[4] );
    ptr[1] = (float)val[6];
    ptr[2] = (float)val[7];
    ptr[3] = (float)val[8];
  } else {
    return 2;
  }

  data.clear();
  val.clear();
  command = "RS,1601W,4";
  if( mqv::Apply( sock, command, receive ) ){
    data = utility::split( receive, ',' );
    val.resize( data.size() );
    for(int i=0, n=data.size(); i<n; ++i ){
      val[i] = std::strtol( data[i].c_str(), NULL, 10 );
    }
    ptr[4] = (float)(val[3] + val[4]*1e4);
  } else {
    return 3;
  }

  return 0;
}

static long init_record(waveformRecord *rec, int pass)
{
  char chost[256];
  char caddr[2];
  if( sscanf( rec->desc, "MQV9500 %s %s", chost, caddr ) != 2 ){
    std::cerr << "MQV9500 no hos ip address or no address" << std::endl;
    return -1;
  }
  host = chost;
  addr = std::strtol( caddr, NULL, 10 );
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

struct IOC devMQV9500={
  6,
  NULL,
  NULL,
  (DEVSUPFUN) init_record,
  NULL,
  (DEVSUPFUN) read_wf,
  NULL
};
epicsExportAddress(dset,devMQV9500);
