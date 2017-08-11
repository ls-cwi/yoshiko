#include "LibraryInterface.h"


using namespace ysk;

namespace yskLib{


	std::string getVersionString(){
		return YOSHIKO_VERSION;
	}

	ysk::ClusterEditingSolutions* processLibraryInput(yskInput::LibraryInput libIn){
		return performAlgorithm(libIn.parseInput());
	}


}

