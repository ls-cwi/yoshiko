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
#include "Globals.h"
#include "CoreAlgorithm.h"

#include "output/ClusterEditingOutput.h"

#include "input/JENAInput.h"
#include "input/SIFInput.h"
#include "input/StreamInput.h"


using namespace std;
using namespace lemon;
using namespace ysk;
using namespace yskInput;

/*
 =========================================================================
 TODO
 =========================================================================
 * validate input data. !!!
 * heuristik, set starting value
 * separation triangles
 * separation partition cuts
 * separation (mueller)
 * modularize program and clean-up god classes
 * enumerating optimal solutions
 * check whether new code produces same sol as old code (in presence of partition constraints)
 * check partition constraints (--> paper!)
 * svn link with Xcode
 * document... how?
 * layout graphs...?
 * Setup unified logging system and streamline all cout//exception calls, apply verbosity globally
 * put in NINA
 * edge index. make faster by directly computing index from i and j. think about (common) solution for directed and undirected graphs
 */


int main(int argc, char * const argv[]) {
  // Initialize the argument parser
  ArgParser ap(argc, argv);
  string inputFilename, outputFilename;
  string graphLabel = "cluster_solution";
  int inputFileFormat = 0; //TODO: Convert to enums for better codestyle
  int outputFileFormat = 0;
  int nrOptimalSolutions = 1;
  bool exportLP = false;
  bool separateTriangles = false;
  bool separatePartitionCuts = false;
  bool useHeuristic = false;
  string rulesBitMask = "111111";
  double multiplicativeFactor = 1;
  
  // Add a string option with storage reference for file name
  ap.refOption("f", "Name of file that contains input []", inputFilename, true);
  //ap.refOption("F", "input file format, 0 = Jena, 1 = Clever, 2 = SIF []", inputFileFormat, false);
  ap.refOption("F", "input file format, 0 = Jena, 1 = SIF [0]", inputFileFormat, false);
  ap.refOption("o", "Name of output file(s) []", outputFilename, false);
  ap.refOption("O", "output file format 0 = csv, 1 = table (line one: number of nodes, line two: number of clusters, column one: node name, column two: cluster ID), 2 = gml, 3 = xgmml (Cytoscape) 4 = Pajek [0], 5 = table (Cytoscape app)", outputFileFormat, false);
  ap.refOption("v", "verbosity, 0 = silent, 5 = full [0]", verbosity, false);
  ap.refOption("H", "utilize heuristic instead of ILP, [false]", useHeuristic, false);
  ap.refOption("T", "CPU time limit (s), -1 = no limit [-1]", time_limit, false);
  ap.refOption("threads", "number of threads [max]", no_threads, false);
  ap.refOption("e", "export LP [false]", exportLP, false);
  ap.refOption("st", "separate triangles [false]", separateTriangles, false);
  ap.refOption("sp", "separate partition cuts [false]", separatePartitionCuts, false);
  ap.refOption("n", "number of optimal solutions [1]", nrOptimalSolutions, false);
  ap.refOption("m", "multiplicative factor for real valued edge weights in SimilarNeighborhoodRule (the higher the better the reduction results and the slower the performance) [1]", multiplicativeFactor, false);
  ap.refOption("g", "graph label []", graphLabel, false);
  ap.refOption("r", "explicitly turn on/off reduction rules, bit string (right to left): bit 0 = CliqueRule, bit 1 = CriticalCliqueRule, bit 2 = AlmostCliqueRule, bit 3 = HeavyEdgeRule3in1, bit 4 = ParameterDependentReductionRule, bit 5 = SimilarNeighborhoodRule [111111]", rulesBitMask, false);
  
  // Perform the parsing process
  // (in case of any error it terminates the program) -> tb improved
  ap.parse();
  
  
  // Check each option if it has been given and print its value
  if (verbosity > 2) {
    
    std::cout << "Parameters of '" << ap.commandName() << "':\n";
    
    std::cout << "      -f: " << inputFilename << std::endl;
    std::cout << "      -F: " << inputFileFormat << std::endl;
    std::cout << "      -o: " << outputFilename << std::endl;
    std::cout << "      -O: " << outputFileFormat << std::endl;
    std::cout << "      -v: " << verbosity << std::endl;
    std::cout << "      -H: " << useHeuristic << std::endl;
    std::cout << "      -T: " << time_limit << std::endl;
    std::cout << "-threads: " << no_threads << std::endl;
    std::cout << "      -e: " << exportLP << std::endl;
    std::cout << "      -st: " << separateTriangles << std::endl;
    std::cout << "      -sp: " << separatePartitionCuts << std::endl;
    std::cout << "      -n: " << nrOptimalSolutions << std::endl;
    std::cout << "      -m: " << multiplicativeFactor << std::endl;
    std::cout << "      -g: " << graphLabel << std::endl;
    std::cout << "      -r: " << rulesBitMask << std::endl;
  }
  
  
  
  ifstream is(inputFilename.c_str());
  if (!is.is_open()) {
    cerr << "file '" << inputFilename << "' not found!" << endl;
    exit(-1);
  }
  

  //Determine which input format is to be parsed and generate a ClusterEditingInstance accordingly

  ClusterEditingInstance* instance = new ClusterEditingInstance();
  StreamInput* input;
  
  switch (inputFileFormat) {
  	  //JENA
  	  case 0:
  		  input = new JENAInput(instance);
  		  break;
  	  case 1:
  		  input = new SIFInput(instance);
  		  break;
  	//case 2:
        //instance.parseCleverFormat(is); <<< ? Clever Format not used :(
        //break;
  	  default:
  		  //Should never be reached
  		  input = new JENAInput(instance);
  		  cout << endl<<"Warning: Input Format not specified, assuming JENA"<<endl;
  		  break;
  }

  if (!input->parseInput(is)){
	  cout << endl << "Parsing failed! Terminating ...";
	  return 1;
  }
  is.close(); //Close input stream
  instance = input->getProblemInstance();
  
  ClusterEditingSolutions* ces = performAlgorithm(
		  	  instance,
			  nrOptimalSolutions,
			  rulesBitMask,
			  multiplicativeFactor,
			  useHeuristic,
			  separatePartitionCuts,
			  separateTriangles
		  );

  //Output generation
  ClusterEditingOutput* output;
  output = ClusterEditingOutput::newInstance(
		  *instance,
		  *ces,
		  outputFilename,
		  graphLabel,
		  outputFileFormat
		  );
  output->write();

  //Final cleanup
  delete instance;
  delete input;
  delete ces;
  delete output;

  
  //Termination
  return 0;
}
