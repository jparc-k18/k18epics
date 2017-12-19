// _*_ C++ _*_

// Author: Shuhei Hayakawa

#include <iostream>

#include "EpicsManager.hh"
#include "HttpServer.hh"

enum eArgv { kProcess, kChannelList, kArgc };

//______________________________________________________________________________
Int_t
main( Int_t argc, Char_t* argv[] )
{
  // gErrorIgnoreLevel = kFatal;

  if( argc != kArgc ){
    std::cout << "Usage: " << ::basename(argv[kProcess])
	      << " [channel_list_file]" << std::endl;
    return EXIT_SUCCESS;
  }

  HttpServer& gHttp = HttpServer::GetInstance();
  gHttp.SetPort( 9090 );

  EpicsManager& gEpics = EpicsManager::GetInstance();
  gEpics.SetFileName( argv[kChannelList] );

  if( !gHttp.Initialize() )
    return EXIT_FAILURE;

  if( !gEpics.Initialize() )
    return EXIT_FAILURE;

  gEpics.Print();

  gEpics.Run();

  return EXIT_SUCCESS;
}
