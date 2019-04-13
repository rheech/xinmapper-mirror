// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define NOMINMAX

#include "targetver.h"
#include <windows.h>
#include <Xinput.h>
#include <process.h>
//#include <intrin.h>

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <utility>
#include <limits>

#include <cmath>
#include <cctype>
#include <cassert>

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

// These two types are really just strings.
typedef std::string ActionDetails;
/*
Type: MapInformation
Info: Created with input from the user, it will
likely be a plain text string describing
how to respond to an ActionDetails.

The string can be tokenized into "words"
separated by spaces, each "word" can be
further subdivided into four parts.

Example:
LTHUMB:LEFT:NORM:a
LTHUMB:RIGHT:NORM:d
LTHUMB:UP:NORM:w
LTHUMB:DOWN:NORM:s

RTRIGGER:NONE:RAPID:1
LTRIGGER:NONE:RAPID:0

They are of the form:
<btn/trigr/thumb>:<more info>:<input sim type>:<value mapped to>
*/
typedef std::string MapInformation;

// Include some commonly used global stuff.
#include "Globals.h"