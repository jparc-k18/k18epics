// -*- C++ -*-

// Author: Shuhei Hayakawa

#ifndef EPICS_DATA_HH
#define EPICS_DATA_HH

#include <vector>

#include <Rtypes.h>
#include <TString.h>

class TGraph;

//_____________________________________________________________________________
class EpicsData
{
public:
  EpicsData( TString name );
  ~EpicsData( void );
  static TString& ClassName( void );

private:
  EpicsData( EpicsData& );
  EpicsData operator =( EpicsData& );

private:
  TString               m_channel_name;
  TGraph*               m_graph;
  std::vector<Int_t>    m_time;
  std::vector<Double_t> m_data;

public:
  TGraph* GetGraph( void ) const { return m_graph; }
  TString GetName( void ) const { return m_channel_name; }
  void    Print( const TString& arg="" ) const;
  Bool_t  Update( void );

};

//_____________________________________________________________________________
inline TString&
EpicsData::ClassName( void )
{
  static TString g_name("EpicsData");
  return g_name;
}

#endif
