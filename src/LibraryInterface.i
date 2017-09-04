//Module that allows basic library interaction
%module LibraryInterface


%javaconst(1);

%typemap(out) long {
    $result = l($1);
}

//Include a direct string-mapping for C++/Java
%include "std_string.i"

%include "ClusterEditingSolutions.i"
%include "LibraryInput.i"
%include "CplexInformer.i"

%{	
	#include "LibraryInterface.h"
	using namespace yskLib;
%}

%ignore CPXSIZE_BITS;

%include "CoreAlgorithm.i"

%include "LibraryInterface.h"