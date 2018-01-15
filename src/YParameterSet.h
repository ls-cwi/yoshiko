/*
 * YParameterSet.h
 *
 *  Created on: Sep 4, 2017
 *      Author: Philipp Spohr
 */

#ifndef SRC_YPARAMETERSET_H_
#define SRC_YPARAMETERSET_H_

namespace ysk {


/**
 * A struct containing all parameters defining a Yoshiko run
 * This is used to have all parameters related to the operation mode in one place
 */
struct YParameterSet {

	/**
	 * Default constructor, sets all operation parameters to their default value
	 * TODO: Define Macro Values to have a more elegant way of changing those default values on the fly
	 */
	YParameterSet()
	:nrOptimalSolutions(1)
	,rulesBitMask("111111")
	,multiplicativeFactor(1)
	,useHeuristic(false),
	separatePartitionCuts(false),
	separateTriangles(false),
	targetClusterCount(-1)
	{};

	/**
	 * The number of solutions that is to be calculated
	 */
	int nrOptimalSolutions;
	/**
	 * This is super akward having a String, would be way better to have a different structure I think, maybe just calculate something and represent as Byte?
	 */
	std::string rulesBitMask;
	double multiplicativeFactor;
	bool useHeuristic;
	bool separatePartitionCuts;
	bool separateTriangles;
	int targetClusterCount;
};

} /* namespace ysk */

#endif /* SRC_YPARAMETERSET_H_ */
