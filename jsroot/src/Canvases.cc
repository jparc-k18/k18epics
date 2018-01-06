// -*- C++ -*-

// Author: Shuhei Hayakawa

#include <TCanvas.h>
#include <TGraph.h>
#include <TROOT.h>

#include "Canvases.hh"
#include "EpicsManager.hh"

//______________________________________________________________________________
namespace
{
  const EpicsManager& gEpics = EpicsManager::GetInstance();

  // local function
  void Draw( TString name, Option_t *option="AL" )
  {
    gEpics.GetGraph( name )->Draw( option );
  }
}

//______________________________________________________________________________
TCanvas*
canvas::ACC( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide( 4, 2 );
  c1->cd(1); Draw("ACC:RUN_NUMBER");
  c1->cd(2); Draw("ACC:HD:INTENSITY");
  c1->cd(3); Draw("ACC:HD:POWER");
  c1->cd(4); Draw("ACC:MR:INTENSITY");
  c1->cd(5); Draw("ACC:MR:POWER");
  c1->cd(6); Draw("ACC:SX:DUTY");
  c1->cd(7); Draw("ACC:SX:EXTRACTION_EFFICIENCY");
  c1->cd(8); Draw("ACC:SX:SPILL_LENGTH");
  return c1;
}

//______________________________________________________________________________
TCanvas*
canvas::AIR( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide( 4, 2 );
  c1->cd(1); Draw("AIR:BFT_TENT:HUMI");
  c1->cd(2); Draw("AIR:BH2_TENT:HUMI");
  c1->cd(3); Draw("AIR:KURAMA_TENT:HUMI");
  c1->cd(4); Draw("AIR:SCH:HUMI");
  c1->cd(5); Draw("AIR:BFT_TENT:TEMP");
  c1->cd(6); Draw("AIR:BH2_TENT:TEMP");
  c1->cd(7); Draw("AIR:KURAMA_TENT:TEMP");
  c1->cd(8); Draw("AIR:SCH:TEMP");
  return c1;
}

//______________________________________________________________________________
TCanvas*
canvas::ESS( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide( 4, 2 );
  c1->cd(1); Draw("ESS1:POS:VMON");
  c1->cd(2); Draw("ESS1:NEG:VMON");
  c1->cd(3); Draw("ESS2:POS:VMON");
  c1->cd(4); Draw("ESS2:NEG:VMON");
  c1->cd(5); Draw("ESS1:POS:IMON");
  c1->cd(6); Draw("ESS1:NEG:IMON");
  c1->cd(7); Draw("ESS2:POS:IMON");
  c1->cd(8); Draw("ESS2:NEG:IMON");
  return c1;
}

//______________________________________________________________________________
TCanvas*
canvas::Field( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide( 1, 2 );
  c1->cd(1); Draw("D4:Field");
  c1->cd(2); Draw("KURAMA:Field");
  return c1;
}

//______________________________________________________________________________
TCanvas*
canvas::Get( const TString& name )
{
  return
    dynamic_cast<TCanvas*>( gROOT->GetListOfCanvases()->FindObject( name ) );
}
