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
  UserSocket *sock = NULL;
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
  bool Read( std::string& buf )
  {
    buf.clear();
    unsigned char c;
    while( sock->Read( &c, 1 ) > 0 ){
      buf += c;
      if( buf.find( Term ) != std::string::npos )
	break;
      if( buf.empty() )
      	return false;
    }
    return true;
  }

  //___________________________________________________________________________
  bool Write( const std::string& msg )
  {
    return ( sock->Write( msg.c_str(), msg.size() ) >= 0 );
  }

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
  bool Apply( const std::string& command, std::string& receive )
  {
#if DEBUG
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
    Write( message );
    std::string buf;
    Read( buf );
    unsigned long cplsumpos = buf.size() - sum.size() - Term.size();
    CplSum( buf, cplsumpos, sum );
    if( ( buf[cplsumpos] != sum[0] ) || ( buf[cplsumpos+1] != sum[1] ) ){
      printf( "CheckSum : %x <-> %x\n", buf[cplsumpos], sum[0] );
      printf( "CheckSum : %x <-> %x\n", buf[cplsumpos+1], sum[1] );
      return false;
    }
    receive = buf.substr( header.str().size(),
			  cplsumpos - header.str().size() - Etx.size() );
    int ret = std::strtol( receive.substr(1, 2).c_str(), NULL, 10 );
    switch( ret ){
    case 0:
      break;
    case 21: case 23:
      std::cerr << "#Waring Something is wrong : " << ret << std::endl;
      break;
    // case 40: case 41: case 43: case 46: case 47: case 48: case 99:
    //   std::cerr << "#ERROR Something is wrong : " << ret << std::endl;
    //   break;
    default:
      std::cerr << "#ERROR Something is wrong : " << ret << std::endl;
      break;
    }
#if DEBUG
    std::cout << "Receive : " << receive << std::endl;
#endif
    return true;
  }
} // namespace mqv

} // namespace

static long read_wf( waveformRecord *rec )
{
  float* ptr = (float*)rec->bptr;
  ptr[0] = ptr[1] = ptr[2] = -9999;
  rec->nord = 3;

  if( sock )
    delete sock;
  sock = new UserSocket( host, 4001 );
  if( !sock->IsOpen() )
    return 1;

  std::string command;
  std::string receive;
  std::vector<std::string> data;
  std::vector<int>         val;

  command = "RS,1201W,8";
  if( mqv::Apply( command, receive ) ){
    data = utility::split( receive, ',' );
    val.resize( data.size() );
    for(int i=0, n=data.size(); i<n; ++i ){
      val[i] = std::strtol( data[i].c_str(), NULL, 10 );
    }
    ptr[0] = (float)( ( val[3]<<2 ) | val[4] );
    ptr[1] = (float)val[6];
    ptr[2] = (float)val[7];
  } else {
    return 2;
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
