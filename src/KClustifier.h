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
	_instance(instance)
	{}
	;

    virtual ~KClustifier();

	void kClustify(unsigned int k, size_t solutionID);

private:

	void mergeCheapest(std::vector<std::vector<int>>& solution);
	double calculateMergeDifference(std::vector<int> cluster1, std::vector<int> cluster2);


	ClusterEditingSolutions* _solutions;
	ClusterEditingInstance* _instance;
};

} /* namespace ysk */

#endif /* SRC_KCLUSTIFIER_H_ */