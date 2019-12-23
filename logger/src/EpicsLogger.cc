// _*_ C++ _*_

// Author: Shuhei Hayakawa

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <signal.h>

#include <TFile.h>
#include <TString.h>
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TTimeStamp.h>
#include <TTree.h>

#include "EpicsLogger.hh"
#include "FuncName.hh"

namespace
{
  Bool_t user_stop = false;

  // local function
  void CatchSignal( int sig )
  {
    ::signal( SIGINT, SIG_IGN );
    std::cout << std::endl << std::endl
	      << "Received SIGINT"
	      << std::endl << std::endl;
    user_stop = true;
  }

  TString Now( void )
  {
    TTimeStamp now;
    now.Add( -TTimeStamp::GetZoneOffset() );
    return now.AsString("s");
  }
}

//_____________________________________________________________________________
EpicsLogger::EpicsLogger( void )
  : m_param_file(),
    m_output_dir(),
    m_channel_list(),
    m_branch_list(),
    m_branch_data(),
    m_file(),
    m_tree(),
    m_now(),
    m_last()
{
}

//_____________________________________________________________________________
EpicsLogger::~EpicsLogger( void )
{
  delete m_tree;
  if( m_file && m_file->IsOpen() ){
    m_file->Close();
  }
  delete m_file;
}

//_____________________________________________________________________________
Bool_t
EpicsLogger::Initialize( void )
{
  std::cout << FUNC_NAME << std::endl
	    << "   ParamFile : " << m_param_file << std::endl
	    << "   OutputDir : " << m_output_dir << std::endl;

  // channel_list_file check
  std::ifstream ifs( m_param_file );
  if( !ifs.good() ){
    std::cerr << " * no such file : " << m_param_file << std::endl;
    return false;
  }

  // output directory check
  TSystemDirectory data_dir( m_output_dir, m_output_dir );
  if( !data_dir.GetListOfFiles() ){
    std::cerr << " * no such directory : " << m_output_dir << std::endl;
    return false;
  }

  // read channel list
  TString line;
  while( ifs.good() && line.ReadLine(ifs) ){
    if( line.IsNull() || line[0]=='#' ) continue;

    // epics channel name
    m_channel_list.push_back(line);

    // tree branch name
    line.ReplaceAll(":", "_");
    m_branch_list.push_back(line);
    m_branch_data.push_back(0);
  }
  ifs.close();

  // print channel list
  std::cout << TString('=', 80) << std::endl;
  for( Int_t i=0, n=m_channel_list.size(); i<n; ++i ){
    std::cout << std::setw(3) << i+1 << " "
	      << m_channel_list[i] << std::endl;
  }
  std::cout << TString('=', 80) << std::endl;

  // create tree
  m_tree = new TTree("tree", "K1.8 EPICS Data");
  m_tree->Branch("UnixTime", &m_now, "UnixTime/I");
  for( Int_t i=0, n=m_branch_list.size(); i<n; ++i ){
    m_tree->Branch(  m_branch_list[i],
		    &m_branch_data[i],
		     m_branch_list[i]+"/D" );
  }

  return true;
}

//_____________________________________________________________________________
void
EpicsLogger::GetEpicsData( void )
{
  for( Int_t i=0, n=m_channel_list.size();
       !user_stop && i<n;
       ++i ){
    m_branch_data[i] = DefaultValue;
    TString cmd("caget -w 10 -t "+m_channel_list[i]);
    FILE *pipe = gSystem->OpenPipe( cmd, "r" );
    if( !pipe ){
      std::cerr << "#E TSystem::OpenPipe() failed : "
		<< cmd << std::endl;
      continue;
    }

    TString input;
    input.Gets( pipe );
    gSystem->ClosePipe( pipe );

    Double_t data1, data2, data3;
    Int_t ret = std::sscanf( input.Data(), "%lf %lf %lf",
			     &data1, &data2, &data3 );
    if( ret==1 ){
      m_branch_data[i] = data1;
    }else if( ret == 3 ){
      m_branch_data[i] = data2;
    }

    ::usleep(10000);
  }
}

//_____________________________________________________________________________
void
EpicsLogger::PrintData( void ) const
{
  std::cout << std::endl << "Time: " << Now() << std::endl;
  for( Int_t i=0, n=m_channel_list.size(); i<n; ++i ){
    std::cout << m_channel_list[i] << " = " << m_branch_data[i] << std::endl;
  }
}

//_____________________________________________________________________________
void
EpicsLogger::PrintTime( void ) const
{
  Int_t remain = NewFileInterval-(m_now-m_last);
  std::cout // << FUNC_NAME
	    << "Last Log Time : " << Now()
	    << " [" << remain << "]" << std::endl;
}

//_____________________________________________________________________________
void
EpicsLogger::Run( void )
{
  std::cout << FUNC_NAME << std::endl;

  ::signal( SIGINT, CatchSignal );

  m_last = std::time(0);

  while( !user_stop ){
    m_now = std::time(0);

    GetEpicsData();
    // PrintData();
    PrintTime();
    m_tree->Fill();

    if( m_now - m_last > NewFileInterval ){
      Write();
      m_last = m_now;
    }

    Int_t rest = LoggingInterval - ( std::time(0) - m_now );
    if( rest>0 ) ::sleep(rest);
  }

  Write();
}

//_____________________________________________________________________________
void
EpicsLogger::Write( void )
{
  TString s = Now();
  s.ReplaceAll(":","");
  s.ReplaceAll("-","");
  s.ReplaceAll(" ","_");

  m_file = new TFile( m_output_dir+"/"+"k18epics_"+s+".root", "RECREATE" );
  m_tree->Write();
  m_file->Close();
  m_tree->Reset();

  std::cout << std::endl << std::endl
	    << "Create ROOT File : " << m_file->GetName()
	    << std::endl << std::endl;
}
