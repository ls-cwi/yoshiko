#include "LibraryInterface.h"


using namespace ysk;

namespace yskLib{

	std::string getVersionString(){
		return YOSHIKO_VERSION;
	}
	void setTimeLimit(int limit){
		time_limit = limit;
	}

	void setThreadLimit(int limit){
		no_threads = limit;
	}

	void setVerbosity(int limit){
		if (limit >= 0 && limit < 4){
			verbosity = limit;
		}
	}

	ysk::CoreAlgorithm* getRun(
			yskInput::LibraryInput* input,
			int nrOptimalSolutions,
			std::string rulesBitMask,
			double multiplicativeFactor,
			bool useHeuristic,
			bool separatePartitionCuts,
			bool separateTriangles,
			int targetClusterCount
	){

		YParameterSet set;
		set.multiplicativeFactor = multiplicativeFactor;
		set.nrOptimalSolutions = nrOptimalSolutions;
		set.rulesBitMask = rulesBitMask;
		set.useHeuristic = useHeuristic;
		set.separatePartitionCuts = separatePartitionCuts;
		set.separateTriangles = separateTriangles;
		set.targetClusterCount = targetClusterCount;

		input->parseInput();

		CoreAlgorithm* ca = new CoreAlgorithm(input->getProblemInstance(),set);

		return ca;
	}


}

