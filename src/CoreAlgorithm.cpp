/*
 * CoreAlgorithm.cpp
 *
 *  Created on: Aug 11, 2017
 *      Author: philipp
 */

#include "CoreAlgorithm.h"

using namespace std;
using namespace lemon;

namespace ysk {
	
	ClusterEditingSolutions* performAlgorithm(
			ClusterEditingInstance* instance,
			int nrOptimalSolutions,
			std::string rulesBitMask, //< Maybe use enums or separate variables internally in the long run? -> More flexibility // maintainability
			double multiplicativeFactor,
			bool useHeuristic,
			bool separatePartitionCuts,
			bool separateTriangles)
	{
		if (verbosity >= 5)
			cout << instance << endl;
	
		// warn user about permanent and forbidden edges
		const WorkingCopyGraph g = instance->getWorkingCopyInstance().getGraph();
		if (verbosity > 1) {
			cout << endl
					<< "Interpreting the following edges as forbidden/permanent: "
					<< endl;
			for (WorkingCopyGraph::EdgeIt e(g); e != INVALID; ++e) {
				if (instance->getWorkingCopyInstance().isForbidden(e))
					cout << instance->getEdgeName(e) << "\tforbidden" << endl;
	
				if (instance->getWorkingCopyInstance().isPermanent(e))
					cout << instance->getEdgeName(e) << "\tpermanent" << endl;
			}
		}
	
		bool conserveMultipleSolutions = false;
	
		if (nrOptimalSolutions > 1) {
			conserveMultipleSolutions = true;
		}
	
		if (verbosity > 1) {
			cout << endl << "applying FPT reduction rules..." << endl;
			cout << "number of nodes:\t"
					<< countNodes(instance->getWorkingCopyInstance().getGraph())
					<< endl;
		}
	
		double totalCost = 0.0;
	
		bitset<NUMBER_OF_REDUCTION_RULES> rules(rulesBitMask);
	
		ClusterEditingReduction cer(
				rules,
				multiplicativeFactor,
				conserveMultipleSolutions
				);
		cer.perform(*instance);
	
		vector<ClusterReductionInstance*>& reduced = cer.getInstances();
	
		if (verbosity > 1) {
			cout << "=========================" << endl;
			cout << "FPT reduction rules applied exhaustively." << endl;
			cout << "time:\t" << clk << endl;
		}
		totalCost += cer.getTotalCost();
	
		if (verbosity > 1) {
			cout << "total cost:\t" << cer.getTotalCost() << endl;
			cout << "number of instances:\t" << reduced.size() << endl;
	
			cout << endl << "==================================" << endl
					<< "==================================" << endl;
			cout << endl << "solving (reduced) instances..." << endl;
			cout << "-------------------------" << endl;
		}
	
		int j = 0;
	
		long totalNumberOfSolutions = 1;
	
		vector<vector<vector<vector<int> > > > instances;
	
		for (vector<ClusterReductionInstance*>::iterator it = reduced.begin();
				it != reduced.end(); it++, j++) {
			if (verbosity > 1) {
				cout << "solving instance 'no " << j << "'..." << endl;
			}
	
			ClusterEditingInstance& i = *(*it)->getInstance();
	
			ClusterEditingSolutions s;
			long numberOfSolutions = 1;
	
			//Solve the remaining and reduced instance either with the heuristic or the ILP
			if (!useHeuristic) {
				//ILP
				try {
					Yoshiko yoshiko(separateTriangles,
							separatePartitionCuts,
							nrOptimalSolutions);
					numberOfSolutions = yoshiko.solve(i, s);
				} catch (IloException &e) {
					cout << "CPLEX error: " << e.getMessage() << endl;
				}
	
			} else {
				//HEURISTIC
				ParameterizedInstance parameterizedInstance(
						i.getWorkingCopyInstance(),
						numeric_limits<double>::signaling_NaN());
				parameterizedInstance.init();
				InducedCostsHeuristic h(parameterizedInstance);
				h.start();
				h.getSolution(s);
			}
	
			if (verbosity > 1)
				cout << "time:\t" << clk << endl;
	
			totalNumberOfSolutions = totalNumberOfSolutions * numberOfSolutions;
	
			totalCost += s.getTotalCost();
	
			//expand solutions: replace merged nodes by cluster
			vector<vector<vector<int> > > partitions;
			expandSolutions(i, s, partitions);
	
			instances.push_back(partitions);
	
			if (verbosity > 1)
				cout << endl << "==================================" << endl
						<< endl;
	
			delete (*it)->getInstance();
		}
		if (verbosity > 1) {
			cout << "all instances solved." << endl;
			cout << "total number of solutions:\t" << totalNumberOfSolutions
					<< endl;
			cout << "total cost (data reduction + ILP or heuristic):\t";
		}
		if (verbosity > 0)
			cout << totalCost << endl;
	
		ClusterEditingSolutions* ces = new ClusterEditingSolutions();
		ces->resize(totalNumberOfSolutions);
	
		size_t k = 0;
		vector<vector<int> > p;
		mergeSolutions(0, k, p, *ces, instances);

		return ces;
	}
	
	void expandSolutions(ClusterEditingInstance& cei,
			ClusterEditingSolutions& ces,
			vector<vector<vector<int> > >& partitions) {
		for (size_t k = 0; k < ces.getNumberOfSolutions(); k++) {
			vector<vector<int> > partition;
			for (size_t l = 0; l < ces.getNumberOfClusters(k); ++l) {
				vector<int> cluster;
				for (vector<int>::const_iterator cit = ces.getCluster(k, l).begin();
						cit != ces.getCluster(k, l).end(); ++cit) {
					cluster.insert(cluster.end(),
							cei.getCluster(cei.getOrig().nodeFromId(*cit))->begin(),
							cei.getCluster(cei.getOrig().nodeFromId(*cit))->end());
				}
				partition.push_back(cluster);
			}
			partitions.push_back(partition);
		}
	}
	
	void mergeSolutions(size_t i, size_t& k, vector<vector<int> >& partition,
			ClusterEditingSolutions& solutions,
			vector<vector<vector<vector<int> > > >& instances) {
		if (i == instances.size()) {
			solutions.getSolution(k).insert(solutions.getSolution(k).end(),
					partition.begin(), partition.end());
			k++;
			return;
		}
		for (size_t j = 0; j < instances[i].size(); j++) {
			vector<vector<int> > p;
			p.insert(p.end(), partition.begin(), partition.end());
			p.insert(p.end(), instances[i][j].begin(), instances[i][j].end());
	
			mergeSolutions(i + 1, k, p, solutions, instances);
		}
	}

}
