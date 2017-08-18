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
			bool useHeuristic,
			bool separatePartitionCuts,
			bool separateTriangles

	){
		libIn->parseInput();
		return performAlgorithm(libIn->getProblemInstance(),nrOptimalSolutions,rulesBitMask,multiplicativeFactor,useHeuristic,separatePartitionCuts,separateTriangles);
	}

	void setTimeLimit(int limit){
		time_limit = limit;
	}

}

