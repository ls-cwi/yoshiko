#include <gtest/gtest.h>

#include <iostream>

#include <lemon/lgf_writer.h>
#include <lemon/adaptors.h>
#include <lemon/bin_heap.h>
#include <lemon/arg_parser.h>

#include "ClusterEditingInstance.h"
#include "ClusterReductionInstance.h"
#include "ClusterEditingSolutions.h"
#include "CoreAlgorithm.h"
#include "Globals.h"
#include "measurement/SilhouetteValue.h"

#include "output/ClusterEditingOutput.h"

#include "input/SIFInput.h"

using namespace std;
using namespace lemon;
using namespace ysk;
using namespace yskInput;


    
TEST(VerifyTestInstancesOptimalSolutions, TestSimple)
{
        bool succesful = false;

	string inputFilename = "../data/test/simple_sif_example.sif";

	YParameterSet parameter;

	ifstream is(inputFilename.c_str());

	if (!is.is_open()) {
		cerr << "file '" << inputFilename << "' not found!" << endl;
		exit(-1);
	}


	//Determine which input format is to be parsed and generate a ClusterEditingInstance accordingly
	ClusterEditingInstance* instance = new ClusterEditingInstance();

	StreamInput* input= new SIFInput(instance);


	if (!input->parseInput(is)){
		std::cout << endl << "Parsing failed! Terminating ...";
		return;
	}
	is.close(); //Close input stream

	instance = input->getProblemInstance();
        
	CoreAlgorithm* core = new CoreAlgorithm(
			instance,
			parameter
	);

	ClusterEditingSolutions* ces = core->run();
        
        if (ces == nullptr){
            cout << "No valid solution found!" << endl;
            return;
        }
        
        if (ces->getFlags().totalCost == 2){
            succesful = true;
        }

	//Final cleanup
	if (verbosity >4) cout << "deleting core algorithm ..." << endl;
	delete core;
        if (verbosity >4) cout << "deleting cluster editing instance ..." << endl;
	delete instance;
        if (verbosity >4) cout << "deleting input ..." << endl;
	delete input;
        if (verbosity >4) cout << "deleting cluster editing solutions ..." << endl;
	delete ces;
 
        EXPECT_TRUE(succesful)<<"Die optimale Lösung mit Kosten 2 wurde nicht gefunden";

}

TEST(VerifyTestInstancesOptimalSolutions, TestMulti)
{

	string inputFilename = "../data/test/trivial_multi_solution.sif";

	YParameterSet parameter;
        parameter.nrOptimalSolutions = 50;
        
	ifstream is(inputFilename.c_str());

	if (!is.is_open()) {
		cerr << "file '" << inputFilename << "' not found!" << endl;
		exit(-1);
	}


	//Determine which input format is to be parsed and generate a ClusterEditingInstance accordingly
	ClusterEditingInstance* instance = new ClusterEditingInstance();

	StreamInput* input= new SIFInput(instance);


	if (!input->parseInput(is)){
		std::cout << endl << "Parsing failed! Terminating ...";
		return;
	}
	is.close(); //Close input stream

	instance = input->getProblemInstance();
        
	CoreAlgorithm* core = new CoreAlgorithm(
			instance,
			parameter
	);

	ClusterEditingSolutions* ces = core->run();
        
        EXPECT_EQ(ces->getNumberOfSolutions(),3);
        
        if (ces == nullptr){
            cout << "No valid solution found!" << endl;
            return;
        }

	//Final cleanup
	if (verbosity >4) cout << "deleting core algorithm ..." << endl;
	delete core;
        if (verbosity >4) cout << "deleting cluster editing instance ..." << endl;
	delete instance;
        if (verbosity >4) cout << "deleting input ..." << endl;
	delete input;
        if (verbosity >4) cout << "deleting cluster editing solutions ..." << endl;
	delete ces;
 
        EXPECT_EQ(ces->getFlags().totalCost,1);
}

