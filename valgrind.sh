#!/bin/bash

#kill old java files
cd ../YoshikoWrapper/src/main/java/de/hhu/ba/yoshikoWrapper/
rm -rf swig
mkdir swig

cd ../../../../../../../../yoshiko/src


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
swig -c++ -java -noproxy -package de.hhu.ba.yoshikoWrapper.swig -outdir ../../YoshikoWrapper/src/main/java/de/hhu/ba/yoshikoWrapper/swig LibraryInterface.i
echo "SWIG END"

cd ../Build
cmake ../
make

valgrind --leak-check=yes ./yoshiko  -f ../../BSc-thesis-graph-based-clustering-app/DataSets/simple_sif_example.sif -F 1

