#include "LibraryInterface.h"


using namespace ysk;

namespace yskLib{


	std::string getVersionString(){
		return YOSHIKO_VERSION;
	}

	ysk::ClusterEditingSolutions* processLibraryInput(
			yskInput::LibraryInput* libIn,
			int nrOptimalSolutions,
			std::string rulesBitMask,
			double multiplicativeFactor,
			bool separatePartitionCuts,
			bool separateTriangles,
			bool useHeuristic

	){
		libIn->parseInput();
		return performAlgorithm(libIn->getProblemInstance(),nrOptimalSolutions,rulesBitMask,multiplicativeFactor,useHeuristic,separatePartitionCuts,separateTriangles);
	}

	void setTimeLimit(int limit){
		time_limit = limit;
	}

	void setVerbosity(int limit){
		if (limit >= 0 && limit < 4){
			verbosity = limit;
		}
	}

}

