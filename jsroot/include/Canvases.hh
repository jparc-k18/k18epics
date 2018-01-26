// -*- C++ -*-

// Author: Shuhei Hayakawa

#ifndef CANVASEC_HH
#define CANVASEC_HH

#include <TString.h>

//______________________________________________________________________________
namespace canvas
{
  TCanvas* ACC( void );
  TCanvas* T1( void );
  TCanvas* AIR( void );
  TCanvas* BGO( void );
  TCanvas* ESS( void );
  TCanvas* Field( void );
  TCanvas* SLIT( void );
  TCanvas* Get( const TString& name );
  void     Update( void );
}

#endif

