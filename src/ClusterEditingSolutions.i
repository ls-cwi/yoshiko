%module ClusterEditingSolutions

%immutable;

%import "Helpers.h"
%import "ClusterEditingInstance.h"
%import "WorkingCopyInstance.h"
%import "Globals.h"

%include "std_vector.i"

namespace std{
%template(IntVector) vector<int>;
}

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