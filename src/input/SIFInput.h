#ifndef SRC_INPUT_SIFINPUT_H_
#define SRC_INPUT_SIFINPUT_H_

#include "StreamInput.h"

namespace yskInput{
	

	class SIFInput: public StreamInput {

		void parseInput(std::istream &is);

	};

}

#endif /* SRC_INPUT_SIFINPUT_H_ */
