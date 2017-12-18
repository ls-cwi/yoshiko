/*
 * KClustifier.cpp
 *
 *  Created on: Dec 18, 2017
 *      Author: philipp
 */

#include "KClustifier.h"

using namespace std;
using namespace lemon;

namespace ysk {

KClustifier::~KClustifier(){};

/**
 * Modifies the given solution so that it consists of k clusters.
 * TODO: Documentation
 * @param k
 * @param solutionID
 */
void KClustifier::kClustify(unsigned int k, size_t solutionID){
	//Fetch the actual solution we want to modify
	vector<vector<int>> solution = _solutions->getSolution(solutionID);
	if (solution.size() == k){
		//We already have the desired amount of clusters
	}
	else if (solution.size()>k){
		//We have too many clusters, time to merge

		//We populate the costMatrix (only upper triangular path is relevant)
		KClustifier::mergeCheapest(solution);
	}

}

void KClustifier::mergeCheapest(vector<vector<int>>& solution){

	double cheapestCost = -INFINITY;
	int mergeIndex1 = -1;
	int mergeIndex2 = -1;

	int indexCluster1 = 0;
	//iterate over clusters
	for(vector<vector<int>>::iterator it = solution.begin(); it != solution.end(); ++it,++indexCluster1) {
		int indexCluster2 = indexCluster1;
		//iterate over other clusters that need to be compared to
		for(vector<vector<int>>::iterator it2 = it; it2 != solution.end(); ++it2,++indexCluster2) {
			if (indexCluster1 == indexCluster2) continue;
			//if (indexCluster1 != 8) continue;
			//We can trivially set the merge difference from a cluster to itself to 0
			double mergeDifference = KClustifier::calculateMergeDifference(*it,*it2);
			if (verbosity > 3){
				cout << "Found a merge-difference of " << mergeDifference << " for clusters " << indexCluster1 << " and " << indexCluster2 <<endl;
			}
			if (mergeDifference > cheapestCost){
				mergeIndex1 = indexCluster1;
				mergeIndex2 = indexCluster2;
				cheapestCost = mergeDifference;
			}
		}
	}
	if (verbosity > 3){
		cout << "The best merge operation would be merging "<<mergeIndex1<<" and "<<mergeIndex2 << " with an associated cost of "<<cheapestCost <<endl;
	}
}

double KClustifier::calculateMergeDifference(vector<int> cluster1, vector<int> cluster2){
	//Initialize the merge difference as 0
	double diff = 0.0;
	//We iterate over all nodes from cluster 1
	for(vector<int>::iterator it = cluster1.begin(); it != cluster1.end(); ++it){
		FullGraph::Node node1 = _instance->getOrig().nodeFromId(*it);
		//And we get the nodes from cluster 2
		for(vector<int>::iterator it2 = cluster2.begin(); it2 != cluster2.end(); ++it2){
			FullGraph::Node node2 = _instance->getOrig().nodeFromId(*it2);
			//Get the edge
			FullGraph::Edge edge = _instance->getOrig().findEdge(node1, node2, INVALID);
			double weight = _instance->getWeight(edge);
			diff += weight;
			if (verbosity > 4){
				cout << "Found relevant edge, weight " << weight << " between nodes " << _instance->getNodeName(node1) << " and " <<  _instance->getNodeName(node2) << endl;
			}
		}
	}
	return diff;
}

} /* namespace ysk */
