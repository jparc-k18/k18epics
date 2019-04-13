// -*- C++ -*-

// Author: Shuhei Hayakawa

#include <TAxis.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TROOT.h>

#include "Canvases.hh"
#include "EpicsManager.hh"

//______________________________________________________________________________
namespace
{
  const EpicsManager& gEpics = EpicsManager::GetInstance();

  // local function
  void Draw( TString name, Option_t *option="L" )
  {
    if( gEpics.GetGraph( name ) )
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
canvas::T1( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide( 2, 2 );
  c1->cd(1); Draw("ACC:HD:T1_MEAN_X");
  c1->cd(2); Draw("ACC:HD:T1_MEAN_Y");
  c1->cd(3); Draw("ACC:HD:T1_SIGMA_X");
  c1->cd(4); Draw("ACC:HD:T1_SIGMA_Y");
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
canvas::BGO( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  // TLegend *l1 = new TLegend( 0.2, 0.2, 0.6, 0.4 );
  gEpics.GetGraph("BGO:CH1")->SetTitle("BGO Temp.");
  for( Int_t i=0; i<20; ++i ){
    if( i==17 || i==18 || i==19 )
      continue;
    Draw( Form("BGO:CH%d", i+1) );
    // l1->AddEntry( gEpics.GetGraph( Form("BGO:CH%d", i+1) )
    // 		  Form("BGO:CH%d", i+1), "L" );
  }
  // l1->Draw();

  return c1;
}

//______________________________________________________________________________
TCanvas*
canvas::ESS( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide( 5, 2 );
  c1->cd( 1); Draw("ESS1:POS:VMON");
  c1->cd( 2); Draw("ESS1:NEG:VMON");
  c1->cd( 3); Draw("ESS2:POS:VMON");
  c1->cd( 4); Draw("ESS2:NEG:VMON");
  c1->cd( 5); Draw("ESS1:CCG");
  c1->cd( 6); Draw("ESS1:POS:IMON");
  c1->cd( 7); Draw("ESS1:NEG:IMON");
  c1->cd( 8); Draw("ESS2:POS:IMON");
  c1->cd( 9); Draw("ESS2:NEG:IMON");
  c1->cd(10); Draw("ESS2:CCG");
  return c1;
}

//______________________________________________________________________________
TCanvas*
canvas::Field( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide(1, 2);
  c1->cd(1); Draw("D4:Field");
  c1->cd(2); Draw("KURAMA:Field");
  return c1;
}

//______________________________________________________________________________
TCanvas*
canvas::GAS( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide(1, 2);
  c1->cd(1); Draw("GAS:SDC2:DIFP");
  c1->cd(2); Draw("GAS:SDC3:DIFP");
  return c1;
}

//______________________________________________________________________________
TCanvas*
canvas::MPPC_SFT( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide(2, 2);
  TString mon[] = { "VSET", "VMON", "IMON", "TEMP" };
  for( Int_t m=0; m<4; ++m ){
    c1->cd(m + 1);
    gEpics.GetGraph(Form("MPPC:SFT:CH0:%s", mon[m].Data()))
      ->SetTitle(Form("MPPC Bias SFT %s", mon[m].Data()));
    for( Int_t i=0; i<8; ++i ){
      Draw( Form("MPPC:SFT:CH%d:%s", i, mon[m].Data()) );
    }
  }
  return c1;
}

//______________________________________________________________________________
TCanvas*
canvas::MPPC_CFT( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide(2, 2);
  TString mon[] = { "VSET", "VMON", "IMON", "TEMP" };
  for( Int_t m=0; m<4; ++m ){
    c1->cd(m + 1);
    gEpics.GetGraph(Form("MPPC:CFT:CH0:%s", mon[m].Data()))
      ->SetTitle(Form("MPPC Bias CFT %s", mon[m].Data()));
    for( Int_t i=0; i<8; ++i ){
      Draw( Form("MPPC:CFT:CH%d:%s", i, mon[m].Data()) );
    }
  }
  return c1;
}

//______________________________________________________________________________
TCanvas*
canvas::SLIT( void )
{
  TCanvas *c1 = new TCanvas(__func__, __func__);
  c1->Divide( 4, 3 );
  c1->cd(1); Draw("SLIT:IFH:LEFT");
  c1->cd(2); Draw("SLIT:IFH:RIGHT");
  c1->cd(3); Draw("SLIT:IFV:LOWER");
  c1->cd(4); Draw("SLIT:IFV:UPPER");
  c1->cd(5); Draw("SLIT:MOM:LEFT");
  c1->cd(6); Draw("SLIT:MOM:RIGHT");
  c1->cd(7);
  c1->cd(8);
  c1->cd(9); Draw("SLIT:MASS1:LOWER");
  c1->cd(10); Draw("SLIT:MASS1:UPPER");
  c1->cd(11); Draw("SLIT:MASS2:LOWER");
  c1->cd(12); Draw("SLIT:MASS2:UPPER");
  return c1;
}

//______________________________________________________________________________
TCanvas*
canvas::Get( const TString& name )
{
  return
    dynamic_cast<TCanvas*>( gROOT->GetListOfCanvases()->FindObject( name ) );
}

//______________________________________________________________________________
void
canvas::Update( void )
{
  {
    Double_t min = 0., max = 0.;
    for( Int_t i=0; i<20; ++i ){
      TGraph *g = gEpics.GetGraph( Form("BGO:CH%d", i+1) );
      Int_t n = g->GetN();
      Double_t* py = g->GetY();
      for( Int_t j=0; j<n; ++j ){
	if( py[j] <= 0.1 ) continue;
	if( py[j] < min ) min = py[j];
	if( max < py[j] ) max = py[j];
      }
    }
    // gEpics.GetGraph("BGO:CH1")->GetYaxis()->SetRangeUser(0.,40.);
    gEpics.GetGraph("BGO:CH1")->GetYaxis()->SetRangeUser( min-1., max+1. );
  }
  TString board[] = { "SFT", "CFT" };
  TString mon[] = { "VSET", "VMON", "IMON", "TEMP" };
  for( Int_t b=0; b<2; ++b ){
    for( Int_t m=0; m<4; ++m ){
      Double_t min = 0., max = 0.;
      for( Int_t i=0; i<8; ++i ){
	TGraph *g = gEpics.GetGraph( Form("MPPC:%s:CH%d:%s",
					  board[b].Data(), i, mon[m].Data()) );
	Int_t n = g->GetN();
	Double_t* py = g->GetY();
	for( Int_t j=0; j<n; ++j ){
	  if( py[j] <= 0.1 ) continue;
	  if( py[j] < min ) min = py[j];
	  if( max < py[j] ) max = py[j];
	}
      }
      gEpics.GetGraph(Form("MPPC:%s:CH0:%s", board[b].Data(), mon[m].Data()))
	->GetYaxis()->SetRangeUser( min-1., max+1. );
    }
  }
  return;
}
