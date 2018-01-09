// -*- C++ -*-

// Author: Shuhei Hayakawa

#include <cstdlib>
#include <iostream>
#include <libgen.h>

#include "EpicsAlarm.hh"

enum eArgv { kProcess, kParamFile, kArgc };

//______________________________________________________________________________
int
main( int argc, char* argv[] )
{
  if( argc!=kArgc ){
    std::cout << "Usage: " << ::basename(argv[kProcess])
	      << " [channel_list_file]" << std::endl;
    return EXIT_SUCCESS;
  }

  epics::Initialize( argv[kParamFile] );
  epics::Run();

  return EXIT_SUCCESS;
}

