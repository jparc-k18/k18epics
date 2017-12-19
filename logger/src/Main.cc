// _*_ C++ _*_

// Author: Shuhei Hayakawa

#include <iostream>

#include <TError.h>

#include "EpicsLogger.hh"

enum eArgv { kProcess, kParamFile, kOutputDir, kArgc };

//______________________________________________________________________________
Int_t
main( Int_t argc, Char_t** argv )
{
  // gErrorIgnoreLevel = kFatal;

  if( argc != kArgc ){
    std::cout << "Usage: " << ::basename(argv[kProcess])
	      << " [ParamFile] [OutputDir]" << std::endl;
    return EXIT_SUCCESS;
  }

  EpicsLogger& gLogger = EpicsLogger::GetInstance();
  gLogger.SetParamFile( argv[kParamFile] );
  gLogger.SetOutputDir( argv[kOutputDir] );

  if( !gLogger.Initialize() )
    return EXIT_FAILURE;

  gLogger.Run();

  return 0;
}
