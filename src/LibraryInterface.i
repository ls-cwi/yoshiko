%module LibraryInterface

%include "std_string.i"
%include "config.h"

%{
#include "LibraryInterface.h"
using namespace yskLib;
%}

%include "LibraryInterface.h"