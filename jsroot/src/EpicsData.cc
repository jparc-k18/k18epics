// -*- C++ -*-

// Author: Shuhei Hayakawa

#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>

#include <TAxis.h>
#include <TGraph.h>
#include <TMath.h>
#include <TSystem.h>

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

//_____________________________________________________________________________
EpicsData::EpicsData( TString name )
  : m_channel_name( name ),
    m_graph( new TGraph ),
    m_time( MaxPoints ),
    m_data( MaxPoints )
{
  m_graph->SetNameTitle( name, name );
  m_graph->SetLineColor( kBlue+1 );
  m_graph->SetLineWidth(3);

  // Color
  if( name.Contains("ACC") ){
    m_graph->SetLineColor( kRed+1 );
  }
  if( name.Contains("ESS2") ){
    m_graph->SetLineColor( kRed+1 );
  }
  if( name.Contains("TEMP") ){
    m_graph->SetLineColor( kRed+1 );
  }
  if( name.Contains("BGO") ){
    name.ReplaceAll("BGO:CH","");
    Int_t ch = name.Atoi();
    Color_t color = ch%10 == 0 ? 11 : ch%10;
    m_graph->SetLineColor( color );
  }
  if( name.Contains("MPPC") ){
    name.ReplaceAll(name(0,11),"");
    Int_t ch = name.Atoi();
    Color_t color = ch%10 == 0 ? 11 : ch%10;
    m_graph->SetLineColor( color );
  }
  if( name.Contains("D4:Field") ){
    m_graph->SetLineColor( kGreen+1 );
  }
  if( name.Contains("KURAMA:Field") ){
    m_graph->SetLineColor( kRed+1 );
  }
  if( name.Contains("CAENHV") ){
    m_graph->SetLineColor( kRed+1 );
  }
  if( name.Contains("SHS:CLG:") ){
    name.ReplaceAll("SHS:CLG:","");
    name.ReplaceAll("TCX","");
    name.ReplaceAll("TSD","");
    name.ReplaceAll("TCC","");
    Int_t ch = name.Atoi();
    Color_t color = (ch%10 == 0 ? 11 : ch%10);
    m_graph->SetLineColor( color );
  }

  std::time_t now = std::time(0);
  for( Int_t i=0; i<MaxPoints; ++i ){
    m_time.at(i) = now
      + gEpics.GetLoggingInterval()*( i - MaxPoints );
    m_data.at(i) = 0.;
  }
  gHttp.Register( m_graph );
}

//_____________________________________________________________________________
EpicsData::~EpicsData( void )
{
  delete m_graph;
  m_graph = nullptr;
}

//_____________________________________________________________________________
void
EpicsData::Print( const TString& arg ) const
{
  std::cout << FUNC_NAME << " " << arg << std::endl
	    << "   channel_name : " << m_channel_name
	    << std::endl;
}

//_____________________________________________________________________________
Bool_t
EpicsData::Update( void )
{
  Double_t val = -9999.;
  TString cmd("caget -w 2 -t "+m_channel_name);
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

  if( TMath::Abs( val ) < 1.e-3  || TMath::IsNaN( val ) )
    val = 0.;
    // return false;

  if( m_channel_name.Contains("CC10") )
    val = TMath::Power( 10, 2*(val - 6.5) );

#ifdef DEBUG
  std::cout << FUNC_NAME << " " << m_channel_name << " " << val << std::endl;
#endif

  m_time.erase( m_time.begin() );
  m_data.erase( m_data.begin() );
  m_time.push_back( std::time(0) );
  m_data.push_back( val );

  m_graph->Set(0);
  for( Int_t i=0; i<MaxPoints; ++i ){
    m_graph->SetPoint( i, m_time.at(i), m_data.at(i) );
  }

  if( m_channel_name.Contains("TEMP") ){
    m_graph->GetYaxis()->SetTitle("degC");
  }
  if( m_channel_name.Contains("CC10") ){
    m_graph->GetYaxis()->SetTitle("Pa");
  }
  if( m_channel_name.Contains("SHS:CLG") ){
    if( m_channel_name.Contains("CI_PS") ){
      m_graph->GetYaxis()->SetTitle("A");
    } else {
      m_graph->GetYaxis()->SetTitle("K");
    }
  }

  m_graph->GetXaxis()->SetTimeDisplay(1);
  // m_graph->GetXaxis()->SetTimeFormat("%Y/%m/%d %H:%M:%S");
  m_graph->GetXaxis()->SetTimeFormat("%m/%d %H:%M");
  // m_graph->GetXaxis()->SetTimeFormat("%H:%M:%S");
  m_graph->GetXaxis()->SetLabelOffset(0.04);
  m_graph->GetXaxis()->SetTimeOffset(0,"jpn");
  m_graph->GetXaxis()->SetNdivisions(-504);

  return true;
}
