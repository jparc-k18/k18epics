// -*- C++ -*-

// Author: Shuhei Hayakawa

#ifndef FUNC_NAME_HH
#define FUNC_NAME_HH

#include <TString.h>

#ifndef FUNC_NAME
#define FUNC_NAME TString("["+ClassName()+"::"+__func__+"()]")
#endif

#endif
