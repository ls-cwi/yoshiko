#!/bin/bash

#kill old java files
cd ../SimpleLoader/src
rm -rf swig
mkdir swig

cd ../../yoshiko/src


#loop over swig-interfaces and process them
#for i in *.i; do
#	swig -c++ -java -noproxy -package de.hhu.ba.yoshikoWrapper.swig -outdir ../../YoshikoWrapper/src/main/java/de/hhu/ba/yoshikoWrapper/swig $i
#	break
#done

#Remove stale cpp wraps
for i in *.cxx; do
	rm $i
done

echo "SWIG START"
swig -c++ -java -noproxy -package swig  -outdir ../../SimpleLoader/src/swig LibraryInterface.i
echo "SWIG END"

cd ../Build
cmake ../
make


echo "Compile Simple Loader"
cd ../../SimpleLoader/src
javac Main.java
cd ../bin
java  Main