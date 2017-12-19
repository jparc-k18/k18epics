// -*- C++ -*-

// Author: Shuhei Hayakawa

#ifndef HTTP_SERVER_HH
#define HTTP_SERVER_HH

#include <vector>

#include <Rtypes.h>
#include <TString.h>

class TGraph;
class THttpServer;

//______________________________________________________________________________
class HttpServer
{
public:
  static HttpServer& GetInstance( void );
  static TString&    ClassName( void );
  ~HttpServer( void );

private:
  HttpServer( void );
  HttpServer( HttpServer& );
  HttpServer operator =( HttpServer& );

private:
  Int_t        m_port;
  THttpServer* m_http;

public:
  Int_t  GetPort( void ) const { return m_port; }
  Bool_t Initialize( void );
  void   Print( const TString& arg="" ) const;
  void   Register( TGraph *graph );
  void   SetPort( Int_t port ){ m_port = port; }

};

//______________________________________________________________________________
inline HttpServer&
HttpServer::GetInstance( void )
{
  static HttpServer g_instance;
  return g_instance;
}

//______________________________________________________________________________
inline TString&
HttpServer::ClassName( void )
{
  static TString g_name("HttpServer");
  return g_name;
}

#endif
