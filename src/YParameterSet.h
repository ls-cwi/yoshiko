#ifndef SRC_YPARAMETERSET_H_
#define SRC_YPARAMETERSET_H_


namespace ysk {

//Default Values for Yoshiko Parameters
const unsigned int DEFAULT_VALUE_OPTIMAL_SOLUTION_COUNT = 1;
const double DEFAULT_VALUE_MULTIPLICATIVE_FACTOR_SNR = 1;
const bool DEFAULT_VALUE_USE_HEURISTIC = false;

/**
 * A struct containing all parameters defining a Yoshiko run
 * This is used to have all parameters related to the operation mode in one place
 */
struct YParameterSet {

	/**
	 * Default constructor, sets all operation parameters to their default value
	 * TODO: Define Const Values to have a more elegant way of changing those default values on the fly
	 * TODO: Make sure that both this file and Globals.h have their correct variables
	 */
	YParameterSet()
	:nrOptimalSolutions(DEFAULT_VALUE_OPTIMAL_SOLUTION_COUNT)
	,rulesBitMask("111111")
	,multiplicativeFactor(DEFAULT_VALUE_MULTIPLICATIVE_FACTOR_SNR)
	,useHeuristic(DEFAULT_VALUE_USE_HEURISTIC),
	separatePartitionCuts(false),
	separateTriangles(false),
	targetClusterCount(-1)
	{};

	/**
	 * The number of solutions that is to be calculated
	 */
	int nrOptimalSolutions; //TODO: (Minor) Should be unsigned? But might not be supported by apref
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
