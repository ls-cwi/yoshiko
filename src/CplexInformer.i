%module(directors="1") CplexInformer

%{
   #include "CplexInformer.h"
   using namespace yskLib;
%}

%feature("director") CplexInformer;

%include "CplexInformer.h"