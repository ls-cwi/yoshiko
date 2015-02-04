/*
 * File:   main.cpp
 * Author: emanuellaude
 *
 * Created on 26. August 2012, 20:07
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>
#include <set>

#include <lemon/full_graph.h>
#include <lemon/lgf_writer.h>
#include <lemon/adaptors.h>
#include <lemon/list_graph.h>
#include <lemon/bin_heap.h>
#include <lemon/arg_parser.h>

#include "ClusterEditingInstance.h"
#include "ClusterEditingOutput.h"
#include "ClusterEditingReduction.h"
#include "ClusterReductionInstance.h"
#include "ClusterEditingSolutions.h"
#include "InducedCostsHeuristic.h"
#include "Yoshiko.h"
#include "Globals.h"


using namespace std;
using namespace lemon;

/*
 =========================================================================
 TODO
 =========================================================================
 * validate input data.
 * heuristik, set starting value
 * separation triangles
 * separation partition cuts
 * separation (mueller)
 * enumerating optimal solutions
 * check whether new code produces same sol as old code (in presence of partition constraints)
 * check partition constraints (--> paper!)
 * svn link with Xcode
 * document... how?
 * layout graphs...?
 * put in NINA
 * edge index. make faster by directly computing index from i and j. think about (common) solution for directed and undirected graphs
 */



//replace reduced cluster by single nodes
void expandSolutions(ClusterEditingInstance& cei, ClusterEditingSolutions& ces, vector<vector<vector<int> > >& partitions);

void mergeSolutions(size_t i, size_t& k, vector<vector<int> >& partition, ClusterEditingSolutions& solutions, vector<vector<vector<vector<int> > > >& instances);

int main(int argc, char * const argv[]) {
	// Initialize the argument parser
	ArgParser ap(argc, argv);
  
	string inputFilename, outputFilename;
  string graphLabel = "cluster_solution";
  int inputFileFormat = 0;
  int outputFileFormat = 0;
  int noOptimalSolutions = 1;
  bool exportLP = false;
  bool separateTriangles = false;
  bool separatePartitionCuts = false;
  bool heuristic = false;
  string rulesBitMask = "111111";
  double multiplicativeFactor = 1;
  
	// Add a string option with storage reference for file name
	ap.refOption("f", "Name of file that contains input []", inputFilename, true);
  ap.refOption("F", "input file format, 0 = Jena, 1 = Clever, 2 = SIF []", inputFileFormat, false);
  ap.refOption("o", "Name of output file(s) []", outputFilename, false);
  ap.refOption("O", "output file format 0 = csv, 1 = table (line one: number of nodes, line two: number of clusters, column one: node name, column two: cluster ID), 2 = gml, 3 = xgmml (Cytoscape) 4 = Pajek [0], 5 = table (Cytoscape app)", outputFileFormat, false);
	ap.refOption("v", "verbosity, 0 = silent, 5 = full [0]", verbosity, false);
  ap.refOption("H", "utilize heuristic instead of ILP, [false]", heuristic, false);
	ap.refOption("T", "CPU time limit (s), -1 = no limit [-1]", time_limit, false);
  ap.refOption("threads", "number of threads [max]", no_threads, false);
	ap.refOption("e", "export LP [false]", exportLP, false);
  ap.refOption("st", "separate triangles [false]", separateTriangles, false);
  ap.refOption("sp", "separate partition cuts [false]", separatePartitionCuts, false);
  ap.refOption("n", "number of optimal solutions [1]", noOptimalSolutions, false);
  ap.refOption("m", "multiplicative factor for real valued edge weights in SimilarNeighborhoodRule (the higher the better the reduction results and the slower the performance) [1]", multiplicativeFactor, false);
  ap.refOption("g", "graph label []", graphLabel, false);
  ap.refOption("r", "explicitly turn on/off reduction rules, bit string (right to left): bit 0 = CliqueRule, bit 1 = CriticalCliqueRule, bit 2 = AlmostCliqueRule, bit 3 = HeavyEdgeRule3in1, bit 4 = ParameterDependentReductionRule, bit 5 = SimilarNeighborhoodRule [111111]", rulesBitMask, false);
  
	// Perform the parsing process
	// (in case of any error it terminates the program)
	ap.parse();
  
  
  // Check each option if it has been given and print its value
  if (verbosity > 2) {
    
    std::cout << "Parameters of '" << ap.commandName() << "':\n";
    
    std::cout << "      -f: " << inputFilename << std::endl;
    std::cout << "      -F: " << inputFileFormat << std::endl;
    std::cout << "      -o: " << outputFilename << std::endl;
    std::cout << "      -O: " << outputFileFormat << std::endl;
    std::cout << "      -v: " << verbosity << std::endl;
    std::cout << "      -H: " << heuristic << std::endl;
    std::cout << "      -T: " << time_limit << std::endl;
    std::cout << "-threads: " << no_threads << std::endl;
    std::cout << "      -e: " << exportLP << std::endl;
    std::cout << "      -st: " << separateTriangles << std::endl;
    std::cout << "      -sp: " << separatePartitionCuts << std::endl;
    std::cout << "      -n: " << noOptimalSolutions << std::endl;
    std::cout << "      -m: " << multiplicativeFactor << std::endl;
    std::cout << "      -g: " << graphLabel << std::endl;
    std::cout << "      -r: " << rulesBitMask << std::endl;
  }
  
  
  
  ifstream is(inputFilename.c_str());
  if (!is.is_open()) {
    cerr << "file '" << inputFilename << "' not found!" << endl;
    exit(-1);
  }
  
	ClusterEditingInstance instance;
	
  
  switch (inputFileFormat) {
    case 0:
      instance.parseJenaFormat(is);
      break;
      
    case 1:
      instance.parseCleverFormat(is);
      break;
    case 2:
      instance.parseSIFFormat(is);
      break;
  }
  
  if (verbosity >= 5)
    cout << instance << endl;
  
  // warn user about permanent and forbidden edges
  const WorkingCopyGraph g = instance.getWorkingCopyInstance().getGraph();
  if (verbosity > 1) {
    cout <<endl<< "interpreting the following edges as forbidden/permanent: " << endl;
    for (WorkingCopyGraph::EdgeIt e(g); e != INVALID; ++e) {
      if (instance.getWorkingCopyInstance().isForbidden(e))
        cout << instance.getEdgeName(e) <<  "\tforbidden" << endl;
      
      if (instance.getWorkingCopyInstance().isPermanent(e))
        cout << instance.getEdgeName(e) <<  "\tpermanent" << endl;
    }
  }
  
  bool conserveMultipleSolutions = false;
  if(noOptimalSolutions > 1) {
    conserveMultipleSolutions = true;
  }
  
  if (verbosity > 1) {
    cout<< endl<<"applying FPT reduction rules..."<<endl;
    cout << "number of nodes:\t" <<countNodes(instance.getWorkingCopyInstance().getGraph())<<endl;
  }
  
  double totalCost = 0.0;
  
  bitset<NUMBER_OF_REDUCTION_RULES> rules(rulesBitMask);
  
  ClusterEditingReduction cer(rules, multiplicativeFactor, conserveMultipleSolutions);
  cer.perform(instance);
  
  
  vector<ClusterReductionInstance*>& reduced = cer.getInstances();
  
  
  if (verbosity > 1) {
    cout<<"========================="<<endl;
    cout <<"FPT reduction rules applied exhaustively."<<endl;
    cout << "time:\t" << clk << endl;
  }
  totalCost += cer.getTotalCost();
  
  if (verbosity > 1) {
    cout <<"total cost:\t"<<cer.getTotalCost()<<endl;
    cout <<"number of instances:\t"<<reduced.size()<<endl;
    
    cout <<endl<<"=================================="<<endl<<"=================================="<<endl;
    cout<<endl<<"solving (reduced) instances..."<<endl;
    cout << "-------------------------"<<endl;
  }
  
  int j = 0;
  
  long totalNumberOfSolutions = 1;
  
  vector<vector<vector<vector<int> > > > instances;
  
  for(vector<ClusterReductionInstance*>::iterator it = reduced.begin(); it != reduced.end(); it++, j++) {
    if (verbosity > 1) {
      cout << "solving instance 'no "<<j<<"'..."<<endl;
    }
    
    ClusterEditingInstance& i = *(*it)->getInstance();
    
    ClusterEditingSolutions s;
    long numberOfSolutions = 1;
    if(!heuristic) {
      try {
	Yoshiko y(separateTriangles, separatePartitionCuts, noOptimalSolutions);
	numberOfSolutions = y.solve(i, s);
      } catch (IloException &e) {
        cout << "CPLEX error: " << e.getMessage() << endl;
      }
      
    } else {
      ParameterizedInstance parameterizedInstance(i.getWorkingCopyInstance(), numeric_limits<double>::signaling_NaN());
      parameterizedInstance.init();
      InducedCostsHeuristic h(parameterizedInstance);
      h.start();
      h.getSolution(s);
    }
    
    if (verbosity > 1)
      cout << "time:\t" << clk << endl;
    
    totalNumberOfSolutions = totalNumberOfSolutions * numberOfSolutions;
    
    totalCost+=s.getTotalCost();
    
    
    //expand solutions: replace merged nodes by cluster
    vector<vector<vector<int> > > partitions;
    expandSolutions(i, s, partitions);
    
    instances.push_back(partitions);
    
    if (verbosity > 1)
      cout <<endl<<"=================================="<<endl<<endl;
    
    delete (*it)->getInstance();
  }
  if (verbosity > 1) {
    cout << "all instances solved."<<endl;
    cout << "total number of solutions:\t"<<totalNumberOfSolutions<<endl;
    cout << "total cost (data reduction + ILP or heuristic):\t";
  }
  if (verbosity > 0) cout <<totalCost<<endl;
  
  
  ClusterEditingSolutions ces;
  ces.resize(totalNumberOfSolutions);
  
  size_t k = 0;
  vector<vector<int> > p;
  mergeSolutions(0, k, p, ces, instances);
  
  ClusterEditingOutput* output = ClusterEditingOutput::newInstance(instance, ces, outputFilename, graphLabel, outputFileFormat);
  output->write();
  delete output;
  
  return 0;
}

void expandSolutions(ClusterEditingInstance& cei, ClusterEditingSolutions& ces, vector<vector<vector<int> > >& partitions) {
  for(size_t k = 0; k < ces.getNumberOfSolutions(); k++) {
    vector<vector<int> > partition;
    for (size_t l = 0; l < ces.getNumberOfClusters(k); ++l) {
      vector<int> cluster;
      for (vector<int>::const_iterator cit = ces.getCluster(k, l).begin(); cit != ces.getCluster(k, l).end(); ++cit) {
        cluster.insert(cluster.end(), cei.getCluster(cei.getOrig().nodeFromId(*cit))->begin(), cei.getCluster(cei.getOrig().nodeFromId(*cit))->end());
      }
      partition.push_back(cluster);
    }
    partitions.push_back(partition);
  }
}

void mergeSolutions(size_t i, size_t& k, vector<vector<int> >& partition, ClusterEditingSolutions& solutions, vector<vector<vector<vector<int> > > >& instances) {
  if(i == instances.size()) {
    solutions.getSolution(k).insert(solutions.getSolution(k).end(), partition.begin(), partition.end());
    k++;
    return;
  }
  for(size_t j = 0; j < instances[i].size(); j++) {
    vector<vector<int> > p;
    p.insert(p.end(), partition.begin(), partition.end());
    p.insert(p.end(), instances[i][j].begin(), instances[i][j].end());
    
    mergeSolutions(i+1, k, p, solutions, instances);
  }
}

