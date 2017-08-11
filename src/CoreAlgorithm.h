#ifndef SRC_COREALGORITHM_H
#define SRC_COREALGORITHM_H

#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>
#include <set>

#include "ClusterEditingSolutions.h"
#include "ClusterEditingReduction.h"
#include "Yoshiko.h"

namespace ysk {

	/**TODO: Complete documentation
	 * Performs the Yoshiko-Algorithm on a given instance with the parameters specified
	 * For an in-depth description of the parameters and the algorithm itself see -> [INSERT LINK TO PAPER HERE?]
	 * @param instance The ClusterEditingInstance that is to be processed
	 * @param nrOptimalSolutions The maximum number of optimal solutions (if multiple ones exist) that are to be calculated
	 * @param rulesBitMask A bit mask that can turn on/off reduction rules TODO: More in-depth explanation
	 * @param multiplicativeFactor
	 * @param useHeuristic
	 * @param separatePartitionCuts
	 * @param separateTriangles
	 * @return
	 */
	ClusterEditingSolutions* performAlgorithm(
			ClusterEditingInstance* instance,
			int nrOptimalSolutions = 1,
			std::string rulesBitMask = "111111",
			double multiplicativeFactor = 1,
			bool useHeuristic = false,
			bool separatePartitionCuts = false,
			bool separateTriangles = false);

	//replace reduced cluster by single nodes
	void expandSolutions(ClusterEditingInstance& cei,
			ClusterEditingSolutions& ces,
			std::vector<std::vector<std::vector<int> > >& partitions);

	void mergeSolutions(size_t i, size_t& k,
			std::vector<std::vector<int> >& partition,
			ClusterEditingSolutions& solutions,
			std::vector<std::vector<std::vector<std::vector<int> > > >& instances);


}

#endif /* SRC_COREALGORITHM_H */
