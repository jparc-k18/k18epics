// _*_ C++ _*_

// Author: Shuhei Hayakawa

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include <TAxis.h>
#include <TCanvas.h>
#include <TError.h>
#include <TFile.h>
#include <TGraph.h>
#include <TString.h>
#include <TTree.h>

#define TREE_DEFAULT_VALUE -999999.999
#define LOGGING_INTERVAL           30   //second
#define NEW_FILE_INTERVAL       14400   //second

namespace
{
  std::vector<std::string> ChannelList;   //epics channel name
  std::vector<std::string> BranchList;    //tree branch name
  std::vector<Double_t> BranchData;         //tree branch data

  TTree *tree=0;
  time_t now;
  time_t last;
  TString output_dir;
  std::vector<TCanvas*> canvas;
  std::vector<TGraph*>  graph;
  const std::size_t NumOfPlot = NEW_FILE_INTERVAL/LOGGING_INTERVAL;
  std::vector< std::vector<Double_t> > PlotData;
  std::vector< std::vector<Double_t> > PlotTime;
  // PlotData[NBranch][NPlot]
  Int_t npoint = 0;
  std::vector<TString> pic_name;

  Bool_t stop_flag = false;
  void sigint_handler( Int_t sig );
  void GetEpicsData( void );
  void PrintData( void );
  void PrintTime( void );
  void WriteRootFile( void );
}

//______________________________________________________________________________
Int_t
main( Int_t argc, char* argv[] )
{
  gErrorIgnoreLevel = kFatal;

  if(argc !=3){
    std::cout << "Usage: " << ::basename(argv[0])
	      << " [channel_list_file] [storage_path]" << std::endl;
    return EXIT_SUCCESS;
  }

  // output directory check
  struct stat st;
  if( stat(argv[2], &st) ){
    std::cerr << "#E no such directory : " << argv[2] << std::endl;
    return EXIT_FAILURE;
  }
  output_dir = argv[2];

  // channel_list_file check
  std::ifstream ifs(argv[1]);
  if( !ifs.good() ){
    std::cerr << "#E no such file : " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  // read channel list
  std::string line;
  while( ifs.good() && std::getline(ifs,line) ){
    std::size_t pos;
    while( (pos=line.find_first_of(" ")) != std::string::npos ){
      line.erase(pos, 1);
    }
    if( line.empty() || line[0]=='#' ) continue;

    // epics channel name
    ChannelList.push_back(line);

    // tree branch name
    std::replace(line.begin(), line.end(), ':', '_');
    BranchList.push_back(line);
    BranchData.push_back(0);

    canvas.push_back( new TCanvas(Form("c_%s", line.c_str()),
				  Form("c_%s", line.c_str()),
				  1000, 800) );
    TGraph *g = new TGraph;
    g->SetTitle(line.c_str());
    g->SetLineColor(kBlue+1);
    g->SetLineWidth(3);
    g->Draw("AL");
    graph.push_back( g );

    pic_name.push_back( TString("pic/"+line+".png") );
  }
  ifs.close();

  // print channel list
  std::cout << "====== EPICS channels =======" << std::endl;
  for( std::size_t i=0; i<ChannelList.size(); ++i ){
    std::cout << std::setw(3)  << i+1 << " "
	      << ChannelList[i] << std::endl;
  }
  std::cout << "=============================" << std::endl;

  // create tree
  tree = new TTree("tree", "K1.8 EPICS Data");
  tree->Branch("UnixTime", &now, "UnixTime/I");
  for( std::size_t i=0; i<BranchList.size(); ++i ){
    tree->Branch( BranchList[i].c_str(),
		  &BranchData[i],
		  Form("%s/D",BranchList[i].c_str()) );
  }

  PlotData.resize( BranchList.size() );
  PlotTime.resize( BranchList.size() );
  for( std::size_t i=0; i<BranchList.size(); ++i ){
    PlotData[i].resize( NumOfPlot );
    PlotTime[i].resize( NumOfPlot );
    for( std::size_t j=0; j<NumOfPlot; ++j ){
      PlotTime[i][j] = std::time(0) - NumOfPlot + j;
    }
  }

  ::signal(SIGINT, sigint_handler);

  last = time(0);

  while( !stop_flag ){
    now = std::time(0);

    GetEpicsData();
    //PrintData();
    PrintTime();
    tree->Fill();

    if( now-last > NEW_FILE_INTERVAL ){
      WriteRootFile();
      last = now;
    }

    Int_t rest = LOGGING_INTERVAL - (time(0) - now);
    if( rest>0 ) sleep(rest);
  }

  WriteRootFile();

  return 0;
}

namespace
{

//______________________________________________________________________________
void
WriteRootFile( void )
{
  char s[256];
  struct tm *p = localtime(&last);
  strftime(s, 256, "%Y%m%d_%H%M%S", p);

  TString filename = output_dir + "/" + "k18epics_" + s + ".root";
  TFile* file = new TFile(filename, "RECREATE");
  tree->Write();
  file->Close();
  tree->Reset();

  std::cout << std::endl << std::endl
	    << "Create ROOT File : " << filename
	    << std::endl << std::endl;
}

//______________________________________________________________________________
void
GetEpicsData( void )
{
  //std::string caget = "caget -w 0.1 -t ";
  std::string caget = "caget -w 30 -t ";
  std::string cmdline;
  FILE* fp;
  char input[128];

  for( std::size_t i=0; i<ChannelList.size(); ++i ){
    BranchData[i] = TREE_DEFAULT_VALUE;
    cmdline = caget+ChannelList[i];
    fp=popen(cmdline.c_str(),"r");
    fgets(input, 128, fp);
    if(pclose(fp) !=0) continue;

    Double_t data1,data2,data3;
    Int_t ret = sscanf(input,"%lf %lf %lf",&data1, &data2, &data3);
    if(ret==1){
      BranchData[i] = data1;
    }else if(ret == 3){
      BranchData[i] = data2;
    }

    graph[i]->Set(0);
    for( std::size_t j=0; j<NumOfPlot-1; ++j ){
      PlotData[i][j] = PlotData[i][j+1];
      PlotTime[i][j] = PlotTime[i][j+1];
      graph[i]->SetPoint(j, PlotTime[i][j], PlotData[i][j]);
    }
    PlotData[i][NumOfPlot-1] = BranchData[i];
    PlotTime[i][NumOfPlot-1] = now;
    graph[i]->SetPoint(NumOfPlot-1, now, BranchData[i]);
    graph[i]->GetXaxis()->SetTimeDisplay(1);
    graph[i]->GetXaxis()->SetLabelOffset(0.04);
    graph[i]->GetXaxis()->SetTimeFormat("#splitline{%Y/%m/%d}{  %H:%M:%S}");
    graph[i]->GetXaxis()->SetTimeOffset(0,"jpg");
    graph[i]->GetXaxis()->SetNdivisions(-503);
    canvas[i]->SetGrid();
    canvas[i]->Print( pic_name[i] );
    ::usleep(10000);
  }
  npoint++;
}

//______________________________________________________________________________
void
PrintData( void )
{
  char s[256];
  struct tm *p = localtime(&now);
  strftime(s, 256, "%Y/%m/%d %H:%M:%S", p);
  std::cout << std::endl << "Time: " << s << std::endl;
  for( std::size_t i=0; i<ChannelList.size(); ++i ){
    std::cout << ChannelList[i] << " = " << BranchData[i] << std::endl;
  }
}

//______________________________________________________________________________
void
PrintTime( void )
{
  char s[256];
  struct tm *p = localtime(&now);
  strftime(s, 256, "%Y/%m/%d %H:%M:%S", p);
  Int_t remain = NEW_FILE_INTERVAL-(now-last);
  std::cout << "Last Log Time : " << s
	    << " [" << remain << "]" << std::endl;
}

//______________________________________________________________________________
void
sigint_handler( int sig )
{
  signal( SIGINT, SIG_IGN );
  std::cout << std::endl << std::endl
	    << "Received SIGINT"
	    << std::endl << std::endl;
  stop_flag = true;
}

}
