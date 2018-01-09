#ifndef SRC_INPUT_SIFINPUT_H
#define SRC_INPUT_SIFINPUT_H

#include "StreamInput.h"

namespace yskInput{
	

	class SIFInput: public StreamInput {
	public:

		SIFInput(ysk::ClusterEditingInstance* inst) : StreamInput(inst){};

		bool parseInput(std::istream &is);

	};

}

#endif /* SRC_INPUT_SIFINPUT_H */
