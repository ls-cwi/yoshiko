#include "LibraryInterface.h"

namespace yskLib{

	std::string getVersionString(){
		return YOSHIKO_VERSION;
	}

	ysk::ClusterEditingInstance* createNewProblemInstance(){
		return new ysk::ClusterEditingInstance();
	}
}

