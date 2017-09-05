#ifndef SOLUTIONFLAGS_H
#define SOLUTIONFLAGS_H


namespace ysk {

	struct SolutionFlags{

		SolutionFlags()
		:totalCost(0.0)
		,timedOut(false)
		,ilpGenerated(false)
		,optimal(false)
		,gapSize(1.0)
		{};

		double totalCost;

		void setTotalCost(double cost){
			totalCost = cost;
		}

		bool timedOut;

		void setTimedOut(bool to){
			timedOut = to;
		}

		bool ilpGenerated;

		void setIlpGenerated(bool generated){
			ilpGenerated = generated;
		}

		bool optimal;

		void setOptimal(bool opt){
			optimal = opt;
		}

		double gapSize;

		void setGap(double gap){
			gapSize = gap;
		}
	};

} // namespace ysk

#endif /* SOLUTIONFLAGS_H */
