/*
 * File:   yoshiko_main.cpp
 * Author: emanuellaude, gunnar klau
 *
 * Created on 26. August 2012, 20:07
 */

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

#include "input/JENAInput.h"
#include "input/SIFInput.h"
#include "input/RowSimInput.h"
#include "input/StreamInput.h"


using namespace std;
using namespace lemon;
using namespace ysk;
using namespace yskInput;

/*
 =========================================================================
 TODO
 =========================================================================
 * validate input data. !!! -> Now part of the separate input classes
 * heuristic, set starting value
 * separation triangles (isnt this already implemented???)
 * separation partition cuts
 * separation (mueller)
 * enumerating optimal solutions (isnt this already implemented?)
 *
 * check whether new code produces same sol as old code (in presence of partition constraints) (Which old code, yosh 1.0?)
 * check partition constraints (--> paper!)
 *
 *
 * svn link with Xcode (still needed?)
 * document... how? -> doxygen. on it!
 * layout graphs...? -> use cytoscape pretty much done
 * Setup unified logging system and streamline all cout//exception calls, apply verbosity globally
 * put in NINA (?)
 * edge index. make faster by directly computing index from i and j. think about (common) solution for directed and undirected graphs
 *
 * K Cluster Approaches
 * Strictly ILP (Add additional constraints)
 * Add k Cluster Centers with M +e Cost (Sum of edge weights)
 * Specify/Choose clusters
 */


int main(int argc, char * const argv[]) {

	//Register time at the beginning of the execution
	clock_t startTime = clock();

	// Initialize the argument parser
	ArgParser ap(argc, argv);
	string inputFilename, outputFilename;
	string graphLabel = "cluster_solution";
	int inputFileFormat = 0;
	int outputFileFormat = 0;
	bool exportLP = false;
	bool printSilhouetteValue = false;

	YParameterSet parameter;

	// Add a string option with storage reference for file name
	ap.refOption("f", "Name of file that contains input []", inputFilename, true);
	//ap.refOption("F", "input file format, 0 = Jena, 1 = Clever, 2 = SIF []", inputFileFormat, false);
	ap.refOption("F", "input file format, 0 = Jena, 1 = SIF, 2 = RowSim [0]", inputFileFormat, false);
	ap.refOption("o", "Name of output file(s) []", outputFilename, false);
	ap.refOption("O", "output file format 0 = csv, 1 = table (line one: number of nodes, line two: number of clusters, column one: node name, column two: cluster ID), 2 = gml, 3 = xgmml (Cytoscape) 4 = Pajek [0], 5 = table (Cytoscape app), 6 = TransClust Format", outputFileFormat, false);
	ap.refOption("v", "verbosity, 0 = silent, 5 = full [0]", verbosity, false);
	ap.refOption("H", "utilize heuristic instead of ILP, [false]", parameter.useHeuristic, false);
	ap.refOption("T", "CPU time limit (s) for the ILP component, -1 = no limit [-1]", time_limit, false);
	ap.refOption("t", "Threshold for RowSim format, Should be in range 0 <= t <= 1 [0.5]",threshold,false);
	ap.refOption("threads", "number of threads [max]", no_threads, false);
	ap.refOption("e", "export LP [false]", exportLP, false);
	ap.refOption("st", "separate triangles [false]", parameter.separateTriangles, false);
	ap.refOption("sp", "separate partition cuts [false]", parameter.separatePartitionCuts, false);
	ap.refOption("n", "number of optimal solutions [1]", parameter.nrOptimalSolutions, false);
	ap.refOption("m", "multiplicative factor for real valued edge weights in SimilarNeighborhoodRule (the higher the better the reduction results and the slower the performance) [1]", parameter.multiplicativeFactor, false);
	ap.refOption("g", "graph label []", graphLabel, false);
	ap.refOption("r", "explicitly turn on/off reduction rules, bit string (right to left): bit 0 = CliqueRule, bit 1 = CriticalCliqueRule, bit 2 = AlmostCliqueRule, bit 3 = HeavyEdgeRule3in1, bit 4 = ParameterDependentReductionRule, bit 5 = SimilarNeighborhoodRule [111111]", parameter.rulesBitMask, false);
	ap.refOption("k", "[EXPERIMENTAL!!!] Define the number of desired clusters, -1 determines this value automatically [-1]",parameter.targetClusterCount,false);
	ap.refOption("s", "[EXPERIMENTAL!!!] Prints the silhouette value at the end of the run",printSilhouetteValue,false);


	// Perform the parsing process
	// (in case of any error it terminates the program) -> tb improved
	ap.parse();

	if (parameter.targetClusterCount != -1) parameter.rulesBitMask = "000000"; // no preprocessing for fixed k

	// Check each option if it has been given and print its value
	if (verbosity > 2) {

		std::cout << "Parameters of '" << ap.commandName() << "':\n";

		std::cout << "      -f: " << inputFilename << std::endl;
		std::cout << "      -F: " << inputFileFormat << std::endl;
		std::cout << "      -o: " << outputFilename << std::endl;
		std::cout << "      -O: " << outputFileFormat << std::endl;
		std::cout << "      -v: " << verbosity << std::endl;
		std::cout << "      -H: " << parameter.useHeuristic << std::endl;
		std::cout << "      -T: " << time_limit << std::endl;
		std::cout << "      -t  " << threshold << std::endl;
		if (ap.given("t") && inputFileFormat != 2){
			std::cout << "Threshold parameter is ignored! Only relevant for SimRow format." << endl;
		}
		std::cout << "-threads: " << no_threads << std::endl;
		std::cout << "      -e: " << exportLP << std::endl;
		std::cout << "      -st: " << parameter.separateTriangles << std::endl;
		std::cout << "      -sp: " << parameter.separatePartitionCuts << std::endl;
		std::cout << "      -n: " << parameter.nrOptimalSolutions << std::endl;
		std::cout << "      -m: " << parameter.multiplicativeFactor << std::endl;
		std::cout << "      -g: " << graphLabel << std::endl;
		std::cout << "      -r: " << parameter.rulesBitMask << std::endl;
		std::cout << "      -k: " << parameter.targetClusterCount << std::endl;
		std::cout << "      -s: " << printSilhouetteValue << std::endl;

	}

	ifstream is(inputFilename.c_str());

	if (!is.is_open()) {
		cerr << "file '" << inputFilename << "' not found!" << endl;
		exit(-1);
	}


	//Determine which input format is to be parsed and generate a ClusterEditingInstance accordingly

	ClusterEditingInstance* instance = new ClusterEditingInstance();
	// if we want the ILP and a fixed number of clusters we generate additional nodes
	if (parameter.useHeuristic == false and parameter.targetClusterCount != -1) instance->set_additional_target_cluster(parameter.targetClusterCount);
	StreamInput* input;


	switch (inputFileFormat) {
		//JENA
		case 0:
			input = new JENAInput(instance);
			break;
			//SIF
		case 1:
			input = new SIFInput(instance);
			break;
			//ROW SIM
		case 2:
			input = new RowSimInput(instance,threshold);
			break;
		default:
			//Should never be reached
			cout << endl<<"Warning: Input Format not specified, assuming JENA"<<endl;
			input = new JENAInput(instance);
			break;
	}

	if (!input->parseInput(is)){
		std::cout << endl << "Parsing failed! Terminating ...";
		return 1;
	}
	is.close(); //Close input stream

	instance = input->getProblemInstance();

	CoreAlgorithm* core = new CoreAlgorithm(
			instance,
			parameter
	);

	ClusterEditingSolutions* ces = core->run();

	//Print Silhouette Value if required
	if (printSilhouetteValue){
		for (unsigned int i = 0; i < ces->getNumberOfSolutions(); i++){
			cout << "Silhouette Value: " << SilhouetteValue(instance,ces->getSolution(i)).getValue() << endl;
		}
	}

	//Output generation
	ClusterEditingOutput* output;
	output = ClusterEditingOutput::newInstance(
			*instance,
			*ces,
			outputFilename,
			graphLabel,
			outputFileFormat
	);

	if (output != nullptr){
		output->write();
	}

	//Final cleanup
	delete core;
	delete instance;
	delete input;
	delete ces;
	delete output;

	if (verbosity > 2) cout << "Total runtime: " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;

	//Termination
	return 0;
}
