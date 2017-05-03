// -*- C++ -*-

#include <algorithm>
#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <libgen.h>
#include <sstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>

#include <TString.h>
#include <TSystem.h>
#include <TTimeStamp.h>

#define Color 1

namespace
{
  const UInt_t CheckInterval = 10; // [s]
  const UInt_t NTime         =  6;
  std::vector<TString>  ChannelList;
  std::vector<Double_t> HighLEVEL;
  std::vector<Double_t> LowLEVEL;
  std::vector< std::vector<Double_t> > value;
  std::vector< std::bitset<NTime> > bit;
  std::time_t now;
}

void CheckEpicsData( void );
void PrintTime( void );
void AlarmSound( void );

//______________________________________________________________________________
Int_t
main(Int_t argc, Char_t* argv[])
{
  const TString& process = ::basename(argv[0]);

  if( argc!=2 ){
    std::cout << "Usage: " << process << " [channel_list_file]" << std::endl;
    return EXIT_SUCCESS;
  }

  std::ifstream ifs(argv[1]);
  if( !ifs.is_open() ){
    std::cerr << "#E No such file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  TString line;
  while( ifs.good() && line.ReadLine(ifs) ){
    if( line[0]=='#' ) continue;
    TString c;
    Double_t p1,p2;
    std::stringstream ss(line.Data());
    if( !( ss >> c >> p1 >> p2 ) )
      continue;
    ChannelList.push_back(c);
    LowLEVEL.push_back(p1);
    HighLEVEL.push_back(p2);
  }
  ifs.close();

  value.resize( ChannelList.size() );
  bit.resize( ChannelList.size() );

  std::cout << TString('=', 60) << std::endl
	    << std::left
	    << std::setw(4) << "CH" << std::setw(20) << "Name"
	    << std::right
	    << std::setw(8) << "Low"
	    << std::setw(8) << "High" << std::endl;
  for( Int_t i=0, n=ChannelList.size(); i<n; ++i ){
    value[i].resize(NTime);
    std::cout << std::left
	      << std::setw(4) << i+1
	      << std::setw(20) << ChannelList[i]
	      << std::right << std::fixed << std::setprecision(1)
	      << std::setw(8) << LowLEVEL[i]
	      << std::setw(8) << HighLEVEL[i] << std::endl;
  }
  std::cout << TString('=', 60) << std::endl;

  while( true ){
    now = std::time(0);

    PrintTime();
    CheckEpicsData();

    for( Int_t i=0, n=value.size()+1; i<n; ++i )
      std::cout << "\033[1A";

    Int_t rest = CheckInterval - (std::time(0) - now);
    if( rest>0 ) ::sleep(rest);
  }

  return EXIT_SUCCESS;
}

//______________________________________________________________________________
void
CheckEpicsData( void )
{
  TString caget = "caget -w 3 -t ";
  TString cmdline;
  FILE* pipe;

  static Int_t count = 0;

  for( Int_t i=0, n=ChannelList.size(); i<n; ++i ){
    cmdline = caget+ChannelList[i];
    pipe = gSystem->OpenPipe(cmdline, "r");
    TString ret;
    ret.Gets(pipe);
    gSystem->ClosePipe(pipe);

    Double_t data;
    if( std::sscanf(ret.Data(), "%lf", &data) != 1)
      continue;

    value[i][count%NTime] = data;
    bit[i].reset(count%NTime);

    if( data < LowLEVEL[i] ){
      bit[i].set(count%NTime);
    }
    if(data > HighLEVEL[i]){
      bit[i].set(count%NTime);
    }
    gSystem->Sleep(10);
  }

  for( Int_t i=0, n=value.size(); i<n; ++i ){
    std::stringstream ss; ss << bit[i];
    TString flag = ss.str();
    flag.ReplaceAll('0','.');
#if Color
    flag.ReplaceAll("1","\033[0;33;1m!\033[0m");
#else
    flag.ReplaceAll('1','!');
#endif
    std::cout << std::right << std::fixed << std::setprecision(1)
	      << ChannelList[i] << " " << flag << " "
	      << std::setw(2) << bit[i].count() << " ";
    for( Int_t j=0, m=value[i].size(); j<m; ++j )
      std::cout << std::setw(6) << value[i][m-j-1];

    std::cout << std::endl;
    if( bit[i].count()==NTime )
      AlarmSound();
  }

  count++;
}

//______________________________________________________________________________
void
PrintTime( void )
{
  TTimeStamp s;
  s.Add( -TTimeStamp::GetZoneOffset() );
  std::cout << "Last update : " << s.AsString("s") << std::endl;
}

//______________________________________________________________________________
void AlarmSound( void )
{
  gSystem->Exec("sh script/call_alarm.sh 2>/dev/null");
}
