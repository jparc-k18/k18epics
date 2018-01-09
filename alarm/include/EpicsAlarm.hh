// -*- C++ -*-

// Author: Shuhei Hayakawa

#ifndef EPICS_ALARM_HH
#define EPICS_ALARM_HH

#include <TString.h>

namespace epics
{
  void AlarmSound( void );
  void CheckEpicsData( void );
  void Initialize( const TString& file_name );
  void PrintTime( void );
  void Run( void );
  void ShowThreshold( void );
}

#endif
