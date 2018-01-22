/*
 * KClustifier.h
 *
 *  Created on: Dec 18, 2017
 *      Author: philipp
 */

#ifndef SRC_KCLUSTIFIER_H_
#define SRC_KCLUSTIFIER_H_

#include "ClusterEditingInstance.h"
#include "ClusterEditingSolutions.h"

#include <lemon/full_graph.h>
#include <lemon/adaptors.h>
#include <lemon/connectivity.h>

namespace ysk {

/**
 * Simple helper construct, describes a separation of a cluster into two clusters as well as the associated editing costs caused by the process of splitting
 */
struct Separation{
	double cost;
	std::vector<int> cluster1;
	std::vector<int> cluster2;
};

/**
 *  Class enabling post-processing to adjust a solution in order to generate a fixed cluster count. This is done by either merging or splitting clusters recursively
 */
class KClustifier {
public:
	/**
	 * Default constructor for KClustifier
	 * @param instance The ClusterEditingInstance on which the KClustifier is to be used
	 * @param solutions The ClusterEditingSolutions from the run
	 */
	KClustifier(ClusterEditingInstance* instance, ClusterEditingSolutions* solutions):
	_solutions(solutions),
	_instance(instance),
	_editingCosts(0)
	{}
	;

	/**
	 * Destructor
	 */
    virtual ~KClustifier();

    /**
     * Transforms the solution identified by the solutionID into k clusters
     * @param k
     * @param solutionID
     */
	void kClustify(unsigned int k, size_t solutionID);

	/**
	 * Returns the costs that were paid for k-clustifying
	 * @return
	 */
	double getCosts();

private:

	/**
	 * Calculates the cost matrix (TODO: Rename variables to clearly identify merging/splitting) for the merge process for a given solution
	 * @param solution The solution
	 */
	void calculateCostMatrix(std::vector<std::vector<int>>& solution);

	/**
	 * Calculates the lower bound split costs for each cluster
	 */
	void calculateLowerBoundSplitCosts(std::vector<std::vector<int>>& solution);

	void mergeCheapest(std::vector<std::vector<int>>& solution);
	double calculateMergeDifference(const std::vector<int> cluster1, const std::vector<int> cluster2);
	void printMergeCosts();

	Separation suggestSeparation(std::vector<int>& cluster);
	void splitCheapest(std::vector<std::vector<int>>& solution);


	std::map<std::pair<int,int>,double> _mergeCosts;
	std::map<int,ysk::Separation> _lowerBoundSplitCosts;
	std::map<lemon::FullGraph::Edge,double> _separationCosts;

	ClusterEditingSolutions* _solutions;
	ClusterEditingInstance* _instance;
	double _editingCosts;

};

} /* namespace ysk */

#endif /* SRC_KCLUSTIFIER_H_ */
