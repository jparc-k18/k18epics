#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include <TFile.h>
#include <TTree.h>

#define TREE_DEFAULT_VALUE -999999.999
#define LOGGING_INTERVAL            30   //second
#define NEW_FILE_INTERVAL        14400   //second

namespace
{
  std::vector<std::string> ChannelList;   //epics channel name
  std::vector<std::string> BranchList;    //tree branch name
  std::vector<double> BranchData;         //tree branch data

  TTree *tree=0;
  time_t now;
  time_t last;
  std::string output_dir;

  volatile int stop_flag=0;
  void sigint_handler( int sig );
  void GetEpicsData( void );
  void PrintData( void );
  void PrintTime( void );
  void WriteRootFile( void );
}

//______________________________________________________________________________
int
main( int argc, char* argv[] )
{
  if(argc !=3){
    std::cout << "Usage: " << basename(argv[0])
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

  ::signal(SIGINT, sigint_handler);

  last = time(0);

  while(stop_flag==0){
    now = time(0);

    GetEpicsData();
    //PrintData();
    PrintTime();
    tree->Fill();

    if( now-last > NEW_FILE_INTERVAL ){
      WriteRootFile();
      last = now;
    }

    int rest = LOGGING_INTERVAL - (time(0) - now);
    if( rest>0 ) sleep(rest);
  }

  WriteRootFile();

  return 0;
}

namespace
{

void WriteRootFile( void )
{
  char s[256];
  struct tm *p = localtime(&last);
  strftime(s, 256, "%Y%m%d_%H%M%S", p);

  std::string filename = output_dir + "/" + "k18epics_" + s + ".root";
  TFile* file = new TFile(filename.c_str(), "RECREATE");
  tree->Write();
  file->Close();
  tree->Reset();

  std::cout << std::endl << std::endl
	    << "Create ROOT File : " << filename
	    << std::endl << std::endl;
}

void GetEpicsData( void )
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

    double data1,data2,data3;
    int ret = sscanf(input,"%lf %lf %lf",&data1, &data2, &data3);
    if(ret==1){
      BranchData[i] = data1;
    }else if(ret == 3){
      BranchData[i] = data2;
    }
    usleep(10000);
  }
}

void PrintData( void )
{
  char s[256];
  struct tm *p = localtime(&now);
  strftime(s, 256, "%Y/%m/%d %H:%M:%S", p);
  std::cout << std::endl << "Time: " << s << std::endl;
  for( std::size_t i=0; i<ChannelList.size(); ++i ){
    std::cout << ChannelList[i] << " = " << BranchData[i] << std::endl;
  }
}

void PrintTime()
{
  char s[256];
  struct tm *p = localtime(&now);
  strftime(s, 256, "%Y/%m/%d %H:%M:%S", p);
  int remain = NEW_FILE_INTERVAL-(now-last);
  std::cout << "Last Log Time : " << s
	    << " [" << remain << "]" << std::endl;
}

void sigint_handler( int sig )
{
  signal( SIGINT, SIG_IGN );
  std::cout << std::endl << std::endl
	    << "Received SIGINT"
	    << std::endl << std::endl;
  stop_flag = -1;
}

}
