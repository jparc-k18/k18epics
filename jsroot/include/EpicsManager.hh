// -*- C++ -*-

// Author: Shuhei Hayakawa

#ifndef EPICS_MANAGER_HH
#define EPICS_MANAGER_HH

#include <vector>

#include <Rtypes.h>
#include <TString.h>

class EpicsData;
class TCanvas;
class TGraph;

//______________________________________________________________________________
class EpicsManager
{
public:
  static EpicsManager& GetInstance( void );
  static TString&      ClassName( void );
  ~EpicsManager( void );

private:
  EpicsManager( void );
  EpicsManager( EpicsManager& );
  EpicsManager operator =( EpicsManager& );

private:
  static const Int_t LoggingInterval = 30; // second
  static const Int_t MaxPoints       = 2*60*12;
  TString                 m_file_name;
  std::vector<EpicsData*> m_data_list;
  std::vector<TCanvas*>   m_canvas_list;

public:
  TCanvas*   GetCanvas( TString name ) const;
  EpicsData* GetEpicsData( TString name ) const;
  TGraph*    GetGraph( TString name ) const;
  Int_t      GetLoggingInterval( void ) const { return LoggingInterval; }
  Int_t      GetMaxPoints( void ) const { return MaxPoints; }
  Bool_t     Initialize( void );
  void       Print( TString arg="" ) const;
  void       PrintTime( void ) const;
  void       Run( void );
  void       SetFileName( TString file_name ) { m_file_name = file_name; }

};

//______________________________________________________________________________
inline EpicsManager&
EpicsManager::GetInstance( void )
{
  static EpicsManager g_instance;
  return g_instance;
}

//______________________________________________________________________________
inline TString&
EpicsManager::ClassName( void )
{
  static TString g_name("EpicsManager");
  return g_name;
}

#endif
