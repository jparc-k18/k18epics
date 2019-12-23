// -*- C++ -*-

// Author: Shuhei Hayakawa

#include <iomanip>
#include <iostream>
#include <fstream>

#include <TGraph.h>
#include <THttpServer.h>
#include <TString.h>

#include "EpicsData.hh"
#include "FuncName.hh"
#include "HttpServer.hh"

//_____________________________________________________________________________
HttpServer::HttpServer( void )
  : m_port(8080),
    m_http()
{
}

//_____________________________________________________________________________
HttpServer::~HttpServer( void )
{
  delete m_http;
  m_http = nullptr;
}

//_____________________________________________________________________________
Bool_t
HttpServer::Initialize( void )
{
  m_http = new THttpServer( Form("http:%d", m_port) );
  m_http->Restrict("/", "allow=all");
  m_http->SetReadOnly(kTRUE);
  m_http->SetJSROOT("https://root.cern.ch/js/latest/");
  m_http->SetTopName("K1.8 EPICS");
  return m_http->IsAnyEngine();
}

//_____________________________________________________________________________
void
HttpServer::Print( const TString& arg ) const
{
  std::cout << FUNC_NAME << " " << arg << std::endl;
  m_http->Print();
}

//_____________________________________________________________________________
void
HttpServer::Register( TGraph *graph )
{
  TString name = graph->GetName();
  name.Remove( name.First(':'), name.Length()-name.First(':') );
  m_http->Register( Form("/%s/", name.Data()), graph);
}
