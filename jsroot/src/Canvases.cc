// -*- C++ -*-

// Author: Shuhei Hayakawa

#include <TCanvas.h>
#include <TGraph.h>

#include "Canvases.hh"
#include "EpicsManager.hh"

//______________________________________________________________________________
namespace
{
  EpicsManager& gEpics = EpicsManager::GetInstance();
}

//______________________________________________________________________________
TCanvas*
canvas::AirHumi( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide( 2, 2 );
  c1->cd(1); gEpics.GetGraph("AIR:BFT_TENT:HUMI")->Draw("AL");
  c1->cd(2); gEpics.GetGraph("AIR:BH2_TENT:HUMI")->Draw("AL");
  c1->cd(3); gEpics.GetGraph("AIR:KURAMA_TENT:HUMI")->Draw("AL");
  c1->cd(4); gEpics.GetGraph("AIR:SCH:HUMI")->Draw("AL");
  return c1;
}

//______________________________________________________________________________
TCanvas*
canvas::AirTemp( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide( 2, 2 );
  c1->cd(1); gEpics.GetGraph("AIR:BFT_TENT:TEMP")->Draw("AL");
  c1->cd(2); gEpics.GetGraph("AIR:BH2_TENT:TEMP")->Draw("AL");
  c1->cd(3); gEpics.GetGraph("AIR:KURAMA_TENT:TEMP")->Draw("AL");
  c1->cd(4); gEpics.GetGraph("AIR:SCH:TEMP")->Draw("AL");
  return c1;
}
