#ifndef SOLUTIONFLAGS_H
#define SOLUTIONFLAGS_H

#include <vector>

namespace ysk {

	/**
	 * Simple struct containing flags that describe what occurred during a run
	 */
	struct SolutionFlags{

		SolutionFlags()
		:totalCost(0.0)
		,timedOut(false)
		,ilpGenerated(false)
		,optimal(true)
		,reducedInstances(0)
		,solvedInstances(0)
		,lastGap(1.0)
		{};

		double totalCost;
		bool timedOut;
		bool ilpGenerated;
		bool optimal;
		int reducedInstances;
		int solvedInstances;
		double lastGap;
	};

} // namespace ysk

#endif /* SOLUTIONFLAGS_H */
