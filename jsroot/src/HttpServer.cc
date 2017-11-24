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

//______________________________________________________________________________
HttpServer::HttpServer( void )
  : m_port(8080),
    m_http()
{
}

//______________________________________________________________________________
HttpServer::~HttpServer( void )
{
}

//______________________________________________________________________________
Bool_t
HttpServer::Initialize( void )
{
  m_http = new THttpServer( Form("http:%d", m_port) );
  m_http->Restrict("/", "allow=all");
  m_http->SetReadOnly(kTRUE);
  return m_http->IsAnyEngine();
}

//______________________________________________________________________________
void
HttpServer::Print( TString arg ) const
{
  std::cout << FUNC_NAME << " " << arg << std::endl;
  m_http->Print();
}

//______________________________________________________________________________
void
HttpServer::Register( TGraph *graph )
{
  TString name = graph->GetName();
  name.Remove( name.First(':'), name.Length()-name.First(':') );
  m_http->Register( Form("/%s/", name.Data()), graph);
}
