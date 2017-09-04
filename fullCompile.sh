#!/bin/bash

#kill old java files
cd ../YoshikoWrapper/src/main/java/de/hhu/ba/yoshikoWrapper/
rm -rf swig
mkdir swig

cd ../../../../../../../../yoshiko/src


#Remove stale cpp wraps
for i in *.cxx; do
	rm $i
done

echo "SWIG START"
swig -c++ -java -package de.hhu.ba.yoshikoWrapper.swig -outdir ../../YoshikoWrapper/src/main/java/de/hhu/ba/yoshikoWrapper/swig LibraryInterface.i
echo "SWIG END"

cd ../Build
cmake ../
make

echo "BUILDING JAVA WRAPPER"
cd ../../YoshikoWrapper
mvn clean install

#Delete all Yoshiko file from cyto repo and install new one for convenience
cd ~/CytoscapeConfiguration/3/apps/installed
rm yoshikoWrapper-0.0.3.jar
cp ~/workspace/cs/BachelorThesis/YoshikoWrapper/target/yoshikoWrapper-0.0.3.jar ~/CytoscapeConfiguration/3/apps/installed


