// -*- C++ -*-

// Author: Shuhei Hayakawa

#include <algorithm>
#include <bitset>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include <TString.h>
#include <TSystem.h>
#include <TTimeStamp.h>

#include "EpicsAlarm.hh"

namespace epics
{

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

//______________________________________________________________________________
void
AlarmSound( void )
{
  gSystem->Exec("sh script/call_alarm.sh 2>/dev/null &");
}

//______________________________________________________________________________
void
CheckEpicsData( void )
{
  static const TString caget = "caget -w 3 -t ";

  for( Int_t i=0, n=ChannelList.size(); i<n; ++i ){
    FILE* pipe = gSystem->OpenPipe(caget+ChannelList[i], "r");
    TString ret;
    ret.Gets(pipe);
    gSystem->ClosePipe(pipe);

    Double_t data = ret.Atof();
    // if( std::sscanf( ret.Data(), "%lf", &data) != 1 ){
    //   continue;
    // }

    for( Int_t j=0, m=NTime-1; j<m; ++j ){
      value[i][j] = value[i][j+1];
      bit[i][j] = bit[i][j+1];
    }

    value[i][NTime-1] = data;
    bit[i].reset(NTime-1);

    if( data < LowLEVEL[i] || HighLEVEL[i] < data )
      bit[i].set(NTime-1);

    gSystem->Sleep(10);
  }

  std::cout << std::left << std::setw(20) << "Name ";
	    // << "Counts ";
  for( Int_t i=0, n=NTime; i<n; ++i )
    std::cout << std::right << std::setw(6)
	      << Form("-%ds", CheckInterval*(n-i-1));
  std::cout << std::endl;

  for( Int_t i=0, n=value.size(); i<n; ++i ){
    std::cout << std::left << std::fixed << std::setprecision(1)
	      << std::setw(20) << ChannelList[i];// << " ";
	      // << std::right
	      // << std::setw(2) << bit[i].count() << "/"
	      // << std::setw(2) << NTime << " ";
    for( Int_t j=0, m=value[i].size(); j<m; ++j ){
      if( bit[i][j] ) std::cout << "\033[0;33;1m";
      std::cout << std::right << std::setw(6)
		<< Form("%.1lf", value[i][j]);
      if( bit[i][j] ) std::cout << "\033[0m";
    }

    // if( bit[i].count()==NTime ){
    if( bit[i][NTime-1] ){
      std::cout << "\033[0;33;1m"
		<< "  !!! Alarm !!!" << "\033[0m";
      AlarmSound();
    }

    std::cout << std::endl;
  }

}

//______________________________________________________________________________
void
Initialize( const TString& file_name )
{
  std::ifstream ifs( file_name );
  if( !ifs.is_open() ){
    std::cerr << "#E No such file " << file_name << std::endl;
    std::exit( EXIT_FAILURE );
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
  for( Int_t i=0, n=ChannelList.size(); i<n; ++i ){
    value[i].resize(NTime);
  }
}

//______________________________________________________________________________
void
PrintTime( void )
{
  TTimeStamp s;
  s.Add( -TTimeStamp::GetZoneOffset() );
  std::cout << "Last update : " << s.AsString("s") << std::endl
	    << std::endl;
}

//______________________________________________________________________________
void
Run( void )
{
  while( true ){
    std::cout << "EPICS Alarm is running ..." << std::endl;
    // std::cout << "\033[2J\033[1;1H" // Clear
    // 	      << "EPICS Alarm is running ..." << std::endl;

    now = std::time(0);

    // ShowThreshold();
    PrintTime();
    CheckEpicsData();

    Int_t rest = CheckInterval - (std::time(0) - now);
    if( rest>0 ) ::sleep(rest);
    // Move cursor
    std::cout << Form("\033[%dA", ChannelList.size()+4) << std::flush;
  }
}

//______________________________________________________________________________
void
ShowThreshold( void )
{
  std::cout << TString('=',5) << " Threshold " << TString('=', 64) << std::endl
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
  std::cout << TString('=', 80) << std::endl;
}

}
