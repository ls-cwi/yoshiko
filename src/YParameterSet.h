/*
 * YParameterSet.h
 *
 *  Created on: Sep 4, 2017
 *      Author: philipp
 */

#ifndef SRC_YPARAMETERSET_H_
#define SRC_YPARAMETERSET_H_

namespace ysk {

struct YParameterSet {


	YParameterSet()
	:nrOptimalSolutions(1)
	,rulesBitMask("111111")
	,multiplicativeFactor(1)
	,useHeuristic(false),
	separatePartitionCuts(false),
	separateTriangles(false)
	{};

	int nrOptimalSolutions;
	std::string rulesBitMask;
	double multiplicativeFactor;
	bool useHeuristic;
	bool separatePartitionCuts;
	bool separateTriangles;
};

} /* namespace ysk */

#endif /* SRC_YPARAMETERSET_H_ */
