#!/bin/bash

cd src
swig -c++ -java -package de.hhu.ba.yoshikoWrapper.swig -outdir ../../YoshikoWrapper/src/main/java/de/hhu/ba/yoshikoWrapper/swig LibraryInterface.i
cd ../Build
cmake ../
make
