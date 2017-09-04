%module LibraryInput

//%import "input/ClusterEditingInput.h"

//Namespace definition
%{
	#include "input/LibraryInput.h"
	using namespace yskInput;
%}


//Only expose what needs to be exposed
%ignore edge;
%ignore parseInput;
%ignore getProblemInstance;

%include "input/LibraryInput.h"
