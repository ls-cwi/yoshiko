#ifndef SOLUTIONFLAGS_H
#define SOLUTIONFLAGS_H

#include <vector>

namespace ysk {

	struct SolutionFlags{

		SolutionFlags()
		:totalCost(0.0)
		,timedOut(false)
		,ilpGenerated(false)
		,optimal(false)
		{};

		double totalCost;
		bool timedOut;
		bool ilpGenerated;
		bool optimal;
		std::vector<double> instances;
	};

} // namespace ysk

#endif /* SOLUTIONFLAGS_H */
