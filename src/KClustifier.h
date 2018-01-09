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
 *  Class enabling post-processing to adjust a solution in order to generate a fixed cluster count.
 */
class KClustifier {
public:
	KClustifier(ClusterEditingInstance* instance, ClusterEditingSolutions* solutions):
	_solutions(solutions),
	_instance(instance),
	_editingCosts(0)
	{}
	;

    virtual ~KClustifier();

	void kClustify(unsigned int k, size_t solutionID);

	/**
	 * Returns the costs that were paid for k-clustifying
	 * @return
	 */
	double getCosts();

private:

	void calculateCostMatrix(std::vector<std::vector<int>>& solution);
	void mergeCheapest(std::vector<std::vector<int>>& solution);
	double calculateMergeDifference(const std::vector<int> cluster1, const std::vector<int> cluster2);


	void printMergeCosts();



	std::map<std::pair<int,int>,double> _mergeCosts;
	ClusterEditingSolutions* _solutions;
	ClusterEditingInstance* _instance;
	double _editingCosts;

};

} /* namespace ysk */

#endif /* SRC_KCLUSTIFIER_H_ */
