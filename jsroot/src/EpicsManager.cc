// -*- C++ -*-

// Author: Shuhei Hayakawa

#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <signal.h>

#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTimeStamp.h>

#include "Canvases.hh"
#include "EpicsData.hh"
#include "EpicsManager.hh"
#include "FuncName.hh"

//_____________________________________________________________________________
namespace
{
  Bool_t user_stop = false;

  // local function
  void CatchSignal( Int_t sig ) {
    ::signal( SIGINT, SIG_IGN );
    std::cout << std::endl << std::endl
	      << "Received SIGINT : " << sig
	      << std::endl << std::endl;
    user_stop = true;
  }
}

//_____________________________________________________________________________
EpicsManager::EpicsManager( void )
  : m_file_name("channel_list.txt"),
    m_data_list(),
    m_canvas_list()
{
}

//_____________________________________________________________________________
EpicsManager::~EpicsManager( void )
{
  for( Int_t i=0, n=m_data_list.size(); i<n; ++i ){
    delete m_data_list.at(i);
    m_data_list.at(i) = nullptr;
  }
  m_data_list.clear();
}

//_____________________________________________________________________________
EpicsData*
EpicsManager::GetEpicsData( const TString& name ) const
{
  for( Int_t i=0, n=m_data_list.size(); i<n; ++i ){
    if( name == m_data_list.at(i)->GetName() )
      return m_data_list.at(i);
  }

  std::cerr << FUNC_NAME << std::endl
	    << " * no such data : " << name << std::endl;
  return nullptr;
}

//_____________________________________________________________________________
TGraph*
EpicsManager::GetGraph( const TString& name ) const
{
  const EpicsData* e = GetEpicsData( name );
  if( e )
    return e->GetGraph();

  std::cerr << FUNC_NAME << std::endl
	    << " * no such data : " << name << std::endl;
  return nullptr;
}

//_____________________________________________________________________________
Bool_t
EpicsManager::Initialize( void )
{
  gStyle->SetPadGridX(true);
  gStyle->SetPadGridY(true);

  std::ifstream ifs( m_file_name );
  if( !ifs.good() ){
    std::cerr << FUNC_NAME << " no such file : " << m_file_name << std::endl;
    return false;
  }

  TString line;
  while( ifs.good() && line.ReadLine(ifs) ){
    if( line[0]=='#' ) continue;
    m_data_list.push_back( new EpicsData( line ) );
  }

  // Canvases
  // m_canvas_list.push_back( canvas::ACC() );
  // m_canvas_list.push_back( canvas::T1() );
  m_canvas_list.push_back( canvas::AIR() );
  // m_canvas_list.push_back( canvas::BGO() );
  // m_canvas_list.push_back( canvas::MPPC_SFT() );
  // m_canvas_list.push_back( canvas::MPPC_CFT() );
  // m_canvas_list.push_back( canvas::SLIT() );
  // m_canvas_list.push_back( canvas::ESS() );
  // m_canvas_list.push_back( canvas::Field() );
  // m_canvas_list.push_back( canvas::GAS() );
  m_canvas_list.push_back( canvas::TPC() );
  m_canvas_list.push_back( canvas::SHSCLG() );
  m_canvas_list.push_back( canvas::SHSLOG() );

  return true;
}

//_____________________________________________________________________________
void
EpicsManager::Print( const TString& arg ) const
{
  std::cout << FUNC_NAME << " " << arg << std::endl;
  for( Int_t i=0, n=m_data_list.size(); i<n; ++i ){
    std::cout << " " << std::setw(3)  << i+1 << " "
  	      << m_data_list.at(i)->GetName() << std::endl;
  }
}

//_____________________________________________________________________________
void
EpicsManager::PrintTime( void ) const
{
  TTimeStamp now;
  now.Add( -TTimeStamp::GetZoneOffset() );
  std::cout // << FUNC_NAME << std::endl
	    << " Last Log : "
	    << now.AsString("s") << std::endl;
}

//_____________________________________________________________________________
void
EpicsManager::Run( void )
{
  ::signal( SIGINT, CatchSignal );

  std::cout << FUNC_NAME << std::endl;

  std::time_t now = 0;
  while( !user_stop && !gSystem->ProcessEvents() ){
    if( std::time(0) - now >= LoggingInterval ){
      for( Int_t i=0, n=m_data_list.size();
	   !user_stop && !gSystem->ProcessEvents() && i<n;
	   ++i ){
	m_data_list.at(i)->Update();
      }
      canvas::Update();
      PrintTime();
      now = std::time(0);
    }
  }
}
