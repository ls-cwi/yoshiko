# Yoshiko

(Weighted) Cluster Editing

Authors: Gunnar Klau, Emanuel Laude, Philipp Spohr

## Compilation instructions

### Dependencies

* LEMON 1.3
* ILOG CPLEX (>= 12.0)

### Compiling

Get yoshiko from github:

    git clone <HTTPS clone URL (see on the right side of this page)>


First, LEMON 1.3 needs to be installed:

    wget http://lemon.cs.elte.hu/pub/sources/lemon-1.3.tar.gz
    tar xvzf lemon-1.3.tar.gz
    cd lemon-1.3
    cmake -DCMAKE_INSTALL_PREFIX=~/lemon
    make install

You can remove the LEMON sources now, i.e., `rm -rf lemon-1.3`.

CPLEX is a commercial product owned by IBM. For academic purposes it can be obtained at no charge via IBM's Academic Initiative programme:

  http://www-03.ibm.com/ibm/university/academic/pub/page/membership

Next, Yoshiko can be compiled:

    mkdir build
    cd build
    cmake ..
    make all

In case auto-detection of LEMON or CPLEX fails, do

    cmake \
    -DLIBLEMON_ROOT=~/lemon \
    -DCPLEX_INC_DIR=~/ILOG/cplex/include/ \
    -DCPLEX_LIB_DIR=~/ILOG/cplex/lib/x86-64_osx/static_pic \
    -DCONCERT_LIB_DIR=~/ILOG/concert/lib/x86-64_osx/static_pic \
    -DCONCERT_INC_DIR=~/ILOG/concert/include/ ..

## CMake-Flags

-DBUILD_STATIC=ON -> Enables static linking

## Make-Targets

executable -> The stand-alone executable of Yoshiko
library -> The library (used for instance by the Cytoscape plugin)
all -> Builds both

## Running Yoshiko (Standalone)
Requires you to build Yoshiko as a standalone program (see Make-Targets)\

To run yoshiko on the test instance (without producing any output):

    ./yoshiko -f ../data/test/fusarium.txt -v 4

Get a list of options:

    ./yoshiko -h
    
## Cytoscape App (Graphical User Interface)

If you are looking for a graphical user interface, you can find a Cytoscape implementation building on this software at:
    
https://gitlab.cs.uni-duesseldorf.de/spohr/YoshikoWrapper

Another stand-alone implementation (that does not require CPLEX) utilizing only the heuristic, is available here:

https://github.com/AlBi-HHU/YoshikoStandalone
