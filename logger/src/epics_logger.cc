// _*_ C++ _*_

// Author: Shuhei Hayakawa

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include <TError.h>
#include <TFile.h>
#include <TString.h>
#include <TSystem.h>
#include <TTree.h>

#define MAKE_ROOT_FILE 0

#define TREE_DEFAULT_VALUE -999999.999
#define LOGGING_INTERVAL           30   //second
#define NEW_FILE_INTERVAL       14400   //second

namespace
{
  enum eArgv { kProcess, kChannelList, kOutputDir, kArgc };
  std::vector<std::string> ChannelList;   //epics channel name
  std::vector<std::string> BranchList;    //tree branch name
  std::vector<Double_t>    BranchData;    //tree branch data

  TTree *tree = 0;
  std::time_t now;
  std::time_t last;
  TString output_dir;

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

  TString process = ::basename(argv[kProcess]);

  if( argc != 3 ){
    std::cout << "Usage: " << process
	      << " [channel_list_file] [storage_path]" << std::endl;
    return EXIT_SUCCESS;
  }

  TString channel_list = argv[kChannelList];
  output_dir = argv[kOutputDir];

  // channel_list_file check
  std::ifstream ifs( channel_list );
  if( !ifs.good() ){
    std::cerr << "#E no such file : " << channel_list << std::endl;
    return EXIT_FAILURE;
  }

  // output directory check
  struct stat st;
  if( ::stat(output_dir, &st) ){
    std::cerr << "#E no such directory : " << output_dir << std::endl;
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
  }
  ifs.close();

  // print channel list
  std::cout << "====== EPICS channels =======" << std::endl;
  for( Int_t i=0, n=ChannelList.size(); i<n; ++i ){
    std::cout << std::setw(3)  << i+1 << " "
	      << ChannelList[i] << std::endl;
  }
  std::cout << "=============================" << std::endl;

  // create tree
  tree = new TTree("tree", "K1.8 EPICS Data");
  tree->Branch("UnixTime", &now, "UnixTime/I");
  for( Int_t i=0, n=BranchList.size(); i<n; ++i ){
    tree->Branch( BranchList[i].c_str(),
		  &BranchData[i],
		  Form("%s/D",BranchList[i].c_str()) );
  }

  ::signal( SIGINT, sigint_handler );

  last = std::time(0);

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
    if( rest>0 ) ::sleep(rest);
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
#if !MAKE_ROOT_FILE
  return;
#endif

  char s[256];
  struct tm *p = std::localtime(&last);
  std::strftime(s, sizeof(s), "%Y%m%d_%H%M%S", p);

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
  for( Int_t i=0, n=ChannelList.size(); i<n; ++i ){
    BranchData[i] = TREE_DEFAULT_VALUE;
    TString cmd("caget -w 10 -t "+ChannelList[i]);
    FILE *pipe = gSystem->OpenPipe( cmd, "r" );
    if( !pipe ){
      std::cerr << "#E TSystem::OpenPipe() failed : "
		<< cmd << std::endl;
      continue;
    }

    TString input;
    input.Gets( pipe );
    gSystem->ClosePipe( pipe );

    Double_t data1, data2, data3;
    Int_t ret = std::sscanf( input.Data(), "%lf %lf %lf",
			     &data1, &data2, &data3 );
    if( ret==1 ){
      BranchData[i] = data1;
    }else if( ret == 3 ){
      BranchData[i] = data2;
    }

    ::usleep(10000);
  }
}

//______________________________________________________________________________
void
PrintData( void )
{
  char s[256];
  struct tm *p = std::localtime(&now);
  std::strftime(s, sizeof(s), "%Y/%m/%d %H:%M:%S", p);
  std::cout << std::endl << "Time: " << s << std::endl;
  for( Int_t i=0, n=ChannelList.size(); i<n; ++i ){
    std::cout << ChannelList[i] << " = " << BranchData[i] << std::endl;
  }
}

//______________________________________________________________________________
void
PrintTime( void )
{
  char s[256];
  struct tm *p = std::localtime(&now);
  std::strftime(s, sizeof(s), "%Y/%m/%d %H:%M:%S", p);
  Int_t remain = NEW_FILE_INTERVAL-(now-last);
  std::cout << "Last Log Time : " << s
	    << " [" << remain << "]" << std::endl;
}

//______________________________________________________________________________
void
sigint_handler( int sig )
{
  ::signal( SIGINT, SIG_IGN );
  std::cout << std::endl << std::endl
	    << "Received SIGINT"
	    << std::endl << std::endl;
  stop_flag = true;
}

}
