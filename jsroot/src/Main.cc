// _*_ C++ _*_

// Author: Shuhei Hayakawa

#include <ctime>
#include <iomanip>
#include <iostream>

#include "EpicsManager.hh"
#include "HttpServer.hh"

enum eArgv { kProcess, kChannelList, kArgc };

//______________________________________________________________________________
Int_t
main( Int_t argc, char* argv[] )
{
  // gErrorIgnoreLevel = kFatal;

  TString process = ::basename(argv[kProcess]);

  if( argc != kArgc ){
    std::cout << "Usage: " << process
	      << " [channel_list_file]" << std::endl;
    return EXIT_SUCCESS;
  }

  HttpServer&   gHttp  = HttpServer::GetInstance();
  gHttp.SetPort( 9090 );
  EpicsManager& gEpics = EpicsManager::GetInstance();
  gEpics.SetFileName( argv[kChannelList] );

  if( !gHttp.Initialize() )
    return EXIT_FAILURE;

  if( !gEpics.Initialize() )
    return EXIT_FAILURE;

  gEpics.Print();

  gEpics.Run();

  return 0;
}
