// -*- C++ -*-

// Author: Shuhei Hayakawa

#ifndef CANVASEC_HH
#define CANVASEC_HH

#include <TString.h>

//______________________________________________________________________________
namespace canvas
{
  TCanvas* ACC( void );
  TCanvas* AIR( void );
  TCanvas* ESS( void );
  TCanvas* Field( void );

  TCanvas* Get( const TString& name );
}

#endif

