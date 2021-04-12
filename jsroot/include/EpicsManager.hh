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

//_____________________________________________________________________________
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
  static const Int_t LoggingInterval = 60; // second
  static const Int_t MaxPoints       = 60*24; // 24 hours
  TString                 m_file_name;
  std::vector<EpicsData*> m_data_list;
  std::vector<TCanvas*>   m_canvas_list;

public:
  EpicsData* GetEpicsData( const TString& name ) const;
  TGraph*    GetGraph( const TString& name ) const;
  Int_t      GetLoggingInterval( void ) const { return LoggingInterval; }
  Int_t      GetMaxPoints( void ) const { return MaxPoints; }
  Bool_t     Initialize( void );
  void       Print( const TString& arg="" ) const;
  void       PrintProcInfo( void ) const;
  void       PrintTime( void ) const;
  void       Run( void );
  void       SetFileName( const TString& file_name ){ m_file_name = file_name; }

};

//_____________________________________________________________________________
inline EpicsManager&
EpicsManager::GetInstance( void )
{
  static EpicsManager g_instance;
  return g_instance;
}

//_____________________________________________________________________________
inline TString&
EpicsManager::ClassName( void )
{
  static TString g_name("EpicsManager");
  return g_name;
}

#endif
