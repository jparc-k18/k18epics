// _*_ C++ _*_

// Author: Shuhei Hayakawa

#ifndef EPICS_LOGGER_HH
#define EPICS_LOGGER_HH

#include <ctime>

#include <Rtypes.h>
#include <TString.h>

class TFile;
class TTree;

//______________________________________________________________________________
class EpicsLogger
{
public:
  static EpicsLogger& GetInstance( void );
  static TString&     ClassName( void );
  ~EpicsLogger( void );

private:
  EpicsLogger( void );
  EpicsLogger( EpicsLogger& );
  EpicsLogger operator =( EpicsLogger& );

private:
  static const Double_t DefaultValue    = -999999.999;
  static const Int_t    LoggingInterval = 30;    //second
  static const Int_t    NewFileInterval = 14400/4; //second

private:
  TString               m_param_file;
  TString               m_output_dir;
  std::vector<TString>  m_channel_list;
  std::vector<TString>  m_branch_list;
  std::vector<Double_t> m_branch_data;
  TFile*                m_file;
  TTree*                m_tree;
  std::time_t           m_now;
  std::time_t           m_last;

public:
  void   GetEpicsData( void );
  Bool_t Initialize( void );
  void   PrintData( void ) const;
  void   PrintTime( void ) const;
  void   Run( void );
  void   SetParamFile( const TString& file ){ m_param_file = file; }
  void   SetOutputDir( const TString& dir ){ m_output_dir = dir; }
  void   Write( void );
};

//______________________________________________________________________________
inline EpicsLogger&
EpicsLogger::GetInstance( void )
{
  static EpicsLogger g_instance;
  return g_instance;
}

//______________________________________________________________________________
inline TString&
EpicsLogger::ClassName( void )
{
  static TString g_name("EpicsLogger");
  return g_name;
}

#endif
