// -*- C++ -*-

// Author: Shuhei Hayakawa

#include <iomanip>
#include <iostream>
#include <fstream>

#include <TAxis.h>
#include <TGraph.h>
#include <TSystem.h>
#include <TTimeStamp.h>

#include "EpicsData.hh"
#include "EpicsManager.hh"
#include "FuncName.hh"
#include "HttpServer.hh"

namespace
{
  EpicsManager& gEpics = EpicsManager::GetInstance();
  HttpServer&   gHttp  = HttpServer::GetInstance();
  const Int_t   MaxPoints = gEpics.GetMaxPoints();
}

//______________________________________________________________________________
EpicsData::EpicsData( TString name )
  : m_channel_name( name ),
    m_graph( new TGraph ),
    m_time( MaxPoints ),
    m_data( MaxPoints )
{
  m_graph->SetNameTitle( name, name );
  m_graph->SetLineWidth(3);

  TTimeStamp now;
  now.Add( -now.GetZoneOffset() );
  for( Int_t i=0; i<MaxPoints; ++i ){
    m_time.at(i) = now.GetSec()
      + gEpics.GetLoggingInterval()*( i - MaxPoints );
    m_data.at(i) = 0.;
  }
  gHttp.Register( m_graph );
}

//______________________________________________________________________________
EpicsData::~EpicsData( void )
{
}

//______________________________________________________________________________
Bool_t
EpicsData::Get( void )
{
  Double_t val = -9999.;
  TString cmd("caget -w 10 -t "+m_channel_name);
  FILE *pipe = gSystem->OpenPipe( cmd, "r" );
  if( !pipe ){
    std::cerr << FUNC_NAME << std::endl << " * "
	      << "TSystem::OpenPipe() failed : "
	      << cmd << std::endl;
    return false;
  }

  TString input;
  input.Gets( pipe );
  gSystem->ClosePipe( pipe );

  Double_t data1, data2, data3;
  Int_t ret = std::sscanf( input.Data(), "%lf %lf %lf",
			   &data1, &data2, &data3 );
  if( ret == 1 )
    val = data1;
  if( ret == 3 )
    val = data2;
  if( ret == 0 || val == -9999. )
    return false;

  if( TMath::Abs( val ) < 1.e-3 )
    val = 0.;

#ifdef DEBUG
  std::cout << FUNC_NAME << " " << m_channel_name << " " << val << std::endl;
#endif

  TTimeStamp now;
  now.Add( -now.GetZoneOffset() );

  m_time.erase( m_time.begin() );
  m_data.erase( m_data.begin() );
  m_time.push_back( now.GetSec() );
  m_data.push_back( val );

  m_graph->Set(0);
  for( Int_t i=0; i<MaxPoints; ++i ){
    m_graph->SetPoint( i, m_time.at(i), m_data.at(i) );
  }

  m_graph->GetXaxis()->SetTimeDisplay(1);
  // m_graph->GetXaxis()->SetTimeFormat("%Y/%m/%d %H:%M:%S");
  m_graph->GetXaxis()->SetTimeFormat("%H:%M:%S");
  m_graph->GetXaxis()->SetLabelOffset(0.04);
  m_graph->GetXaxis()->SetTimeOffset(0,"jpn");
  // m_graph->GetXaxis()->SetNdivisions(-503);
  return true;
}

//______________________________________________________________________________
void
EpicsData::Print( TString arg ) const
{
  std::cout << FUNC_NAME << " " << arg << std::endl
	    << "   channel_name : " << m_channel_name
	    << std::endl;
}
