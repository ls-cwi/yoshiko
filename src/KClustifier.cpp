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
	_editingCosts = 0;
	//Fetch the actual solution we want to modify
	vector<vector<int>>& solution = _solutions->getSolution(solutionID);
	if (solution.size() == k){
		//We already have the desired amount of clusters
	}
	else if (solution.size()>k){
		//We have too many clusters, time to merge
		if (verbosity > 3){
			cout << "Too many clusters generated (" << solution.size() << "), we need: " << k <<endl;
		}
		//We populate the costMatrix (only upper triangular path is relevant)
		KClustifier::calculateCostMatrix(solution);
		if (verbosity > 4 ){
			KClustifier::printMergeCosts();
		}
		//We then simply merge until we reach the required number of clusters
		while (solution.size()>k){
			KClustifier::mergeCheapest(solution);
			if (verbosity > 4 ){
				KClustifier::printMergeCosts();
				_solutions->printSolution(solutionID);
			}
		}
	}
	else if (solution.size()<k){
		//We don't have enough clusters, time to split!
		if (verbosity > 3){
			cout << "Not enough clusters generated (" << solution.size() << "), we need: " << k <<endl;
		}
		KClustifier::calculateLowerBoundSplitCosts(solution);
	}
}

void KClustifier::mergeCheapest(vector<vector<int>>& solution){
	//Generate a new mergeCost map
	std::map<std::pair<int,int>,double> newMergeCosts = std::map<std::pair<int,int>,double>();
	//Find minimum
	using pair_type = decltype(_mergeCosts)::value_type;
	std::pair<int,int> targetClusters = std::max_element(_mergeCosts.begin(),_mergeCosts.end(),
			 [] (const pair_type & p1, const pair_type & p2) {
			        return p1.second < p2.second;
			    }
	)->first;
	if (verbosity > 3){
		cout << "Merging clusters " << targetClusters.first << " and " << targetClusters.second << endl;
	}
	//Add the second vector to the first
	solution[targetClusters.first].insert(solution[targetClusters.first].end(),solution[targetClusters.second].begin(),solution[targetClusters.second].end());
	//Delete the second cluster

	solution.erase(solution.begin()+targetClusters.second);

	//Adjust editing costs
	_editingCosts += _mergeCosts[targetClusters];

	//Adjust merge costs (for all indices beyond the one of the second cluster)
	for (auto const &entry : _mergeCosts){

		//We skip entries for the cluster that just got deleted
		if (entry.first.first == targetClusters.second || entry.first.second == targetClusters.second) continue;

		std::pair<int,int> index = entry.first;
		double costs = entry.second;

		//All cluster indices beyond the deleted one need to be decremented
		if (entry.first.first > targetClusters.second){
			index.first --;
		}
		if (entry.first.second > targetClusters.second){
			index.second --;
		}
		//All entries from the first cluster need to be updated
		if (entry.first.first == targetClusters.first){
			//We look for the other entry and add them
			for (auto const &entry2 : _mergeCosts){
				if (entry2.first.first == targetClusters.second && entry2.first.second == entry.first.second){
					costs += entry2.second;
					break;
				}
				if (entry2.first.second == targetClusters.second && entry2.first.first == entry.first.first){
					costs += entry2.second;
					break;
				}
			}
		}
		//All entries to the first cluster need to be updated
		else if (entry.first.second == targetClusters.first){
			//We look for the other entry and add them
			for (auto const &entry2 : _mergeCosts){
				if (entry2.first.second == targetClusters.second && entry2.first.first == entry.first.first){
					costs += entry2.second;
					break;
				}
				if (entry2.first.first == targetClusters.second && entry2.first.second == entry.first.second){
					costs += entry2.second;
					break;
				}
			}
		}
		newMergeCosts[index] = costs;
	}
	//Replace the old merge costs
	_mergeCosts = newMergeCosts;
}

void KClustifier::calculateCostMatrix(vector<vector<int>>& solution){
	//initialize empty map used for storing the merging costs
	_mergeCosts = std::map<std::pair<int,int>,double>();

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
			_mergeCosts[std::make_pair(indexCluster1,indexCluster2)] = mergeDifference;
			if (verbosity > 3){
				cout << "Found a merge-difference of " << mergeDifference << " for clusters " << indexCluster1 << " and " << indexCluster2 <<endl;
			}

		}
	}

}

void KClustifier::calculateLowerBoundSplitCosts(vector<vector<int>>& solution){
	_lowerBoundSplitCosts = std::map<int,Separation>();

	int indexCluster = 0;

	//iterate over clusters
	for(vector<vector<int>>::iterator it = solution.begin(); it != solution.end(); ++it, ++ indexCluster) {
		Separation suggestedSeparation = KClustifier::suggestSeparation(*it);
		if (verbosity > 4){
			cout << "Suggested Split for cluster " << indexCluster << ":" << endl;

			cout << "Nodes: ";
			for (vector<int>::iterator sep1 = suggestedSeparation.cluster1.begin(); sep1 != suggestedSeparation.cluster1.end(); ++sep1){
				cout << _instance->getNodeName(_instance->getOrig().nodeFromId(*sep1));
			}
			cout << endl;

			cout << "Nodes: ";
			for (vector<int>::iterator sep2 = suggestedSeparation.cluster2.begin(); sep2 != suggestedSeparation.cluster2.end(); ++sep2){
				cout << _instance->getNodeName(_instance->getOrig().nodeFromId(*sep2));
			}
			cout << endl;

			cout << "Cost: " << suggestedSeparation.cost << endl << endl;
		}
		_lowerBoundSplitCosts[indexCluster] = suggestedSeparation;
	}
}

/**
 * Suggests a possible split based on the InducedCostsOfSeparation heuristic
 */
Separation KClustifier::suggestSeparation(vector<int>& cluster){

	//TODO: Speed-Up AND/OR Improve

	//This is what we will return
	Separation suggestion;
	suggestion.cost = std::numeric_limits<double>::infinity();

	//Check all edges in the cluster and assume them as separators
	for (vector<int>::iterator it = cluster.begin(); it != cluster.end(); ++it){
		FullGraph::Node node1 = _instance->getOrig().nodeFromId(*it);
		for (vector<int>::iterator it2 = it; it2 != cluster.end(); ++it2){

			//We initialize a new separation
			Separation separation;
			separation.cost = 0;

			//We fetch the lemon objects for quicker reference
			FullGraph::Node node2 = _instance->getOrig().nodeFromId(*it2);
			FullGraph::Edge edge = _instance->getOrig().findEdge(node1, node2, INVALID);

			//We assume that those two nodes are in different clusters
			//We therefore put them both into different clusters in the suggested separation
			separation.cluster1.push_back(*it);
			separation.cluster2.push_back(*it2);
			//We will also pay the costs of removing the edge
			separation.cost += _instance->getWeight(edge);

			//We then check for each other node what the cheapest option would be, ignoring relationships between the nodes
			for (vector<int>::iterator it3 = cluster.begin();it3 != cluster.end(); ++it3){
				if (it3 == it || it3 == it2) continue; //We don't want that
				//Fetch lemon objects and associated edge weights
				FullGraph::Node node3 = _instance-> getOrig().nodeFromId(*it3);
				FullGraph::Edge from1to3 = _instance->getOrig().findEdge(node1, node3, INVALID);
				FullGraph::Edge from2to3 = _instance->getOrig().findEdge(node2, node3, INVALID);

				double costFrom1to3 = _instance->getWeight(from1to3);
				double costFrom2to3 = _instance->getWeight(from2to3);

				//TODO: Here could be a good place to start improving this heuristic
				//For instance, the case = could be treated differently with a more in-depth calculation, recognizing more connections

				//Pick the cheaper option

				if (costFrom1to3 > costFrom2to3){
					//Keep the Connection 1--3, remove the Connection 2--3
					separation.cluster1.push_back(*it3);
					separation.cost += costFrom2to3;
				}

				if (costFrom1to3 <= costFrom2to3){ //TODO: Reduce to only <
					//Keep the Connection 2--3, remove the Connection 1--3
					separation.cluster2.push_back(*it3);
					separation.cost += costFrom1to3;
				}


			}
			//If it is good, we keep it!
			if (separation.cost < suggestion.cost){
				suggestion = separation;
			}
		}
	}

	return suggestion;
}

/**
 * Calculates the editing costs that would be associated with merging two clusters
 * @param cluster1 One of the two clusters that are to be merged
 * @param cluster2 One of the two clusters that are to be merged
 * @return The editing costs as double
 */
double KClustifier::calculateMergeDifference(vector<int> cluster1,vector<int> cluster2){
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

void KClustifier::printMergeCosts(){
	cout << "Merge Cost Overview:" << endl << endl;
	for (auto const &entry : _mergeCosts){
		cout << entry.first.first  << ",";
		cout << entry.first.second << ": ";
		cout << entry.second << endl;
	}
	cout << endl;
}

double KClustifier::getCosts(){
	return _editingCosts;
}

} /* namespace ysk */
