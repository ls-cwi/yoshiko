#include "LibraryInterface.h"


using namespace ysk;

namespace yskLib{


	std::string getVersionString(){
		return YOSHIKO_VERSION;
	}

	ysk::ClusterEditingSolutions* processLibraryInput(yskInput::LibraryInput* libIn){
		libIn->parseInput();
		return performAlgorithm(libIn->getProblemInstance());
	}

}

