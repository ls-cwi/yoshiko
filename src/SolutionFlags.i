%module SolutionFlags

%{
	#include "SolutionFlags.h"
%}

%ignore setTotalCost;
%ignore setTimedOut;
%ignore setIlpGenerated;
%ignore setOptimal;
%ignore setGap;

%include "SolutionFlags.h"