//Module that allows basic library interaction
%module LibraryInterface

//Include a direct string-mapping for C++/Java
%include "std_string.i"

//Include basic wrapping for ClusterEditingInstance
%include "ClusterEditingInstance.i"

//Namespace definition
%{
	#include "LibraryInterface.h"
	using namespace yskLib;
%}

%include "LibraryInterface.h"