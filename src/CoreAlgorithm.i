%module CoreAlgorithm


//Namespace definition
%{
	#include "CoreAlgorithm.h"
%}

%ignore CoreAlgorithm(
			ClusterEditingInstance* instance,
			YParameterSet parameter
);

%ignore mergeSolutions;
%ignore expandSolutions;

%include "CoreAlgorithm.h"