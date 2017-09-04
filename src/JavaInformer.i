%module(directors="1") JavaInformer

%{
   #include "JavaInformer.h"
   using namespace yskLib;
%}

%feature("director") JavaInformer;

%include "JavaInformer.h"