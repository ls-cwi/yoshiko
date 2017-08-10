%module LibraryInput

%import "input/ClusterEditingInput.h"

//Namespace definition
%{
	#include "input/LibraryInput.h"
	using namespace yskInput;
%}


%include "input/LibraryInput.h"
