%module ClusterEditingSolutions

%immutable;

%import "Helpers.h"
%import "ClusterEditingInstance.h"
%import "WorkingCopyInstance.h"
%import "Globals.h"

%include "std_vector.i"

//Namespace definition
%{
	#include "ClusterEditingSolutions.h"
	using namespace ysk;
%}

//HIDE some functions
%ignore resize;
%ignore setSolution;
%ignore setTotalCost;
  
%include "ClusterEditingSolutions.h"