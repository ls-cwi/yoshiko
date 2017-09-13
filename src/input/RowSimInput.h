/*
 * RowSimInput.h
 *
 *  Created on: Sep 13, 2017
 *      Author: philipp
 */

#ifndef SRC_INPUT_ROWSIMINPUT_H_
#define SRC_INPUT_ROWSIMINPUT_H_

#include "StreamInput.h"
#include "float.h"

namespace yskInput {
/**
 * This format is used to describe similarities between objects,
 * It consists of a number of rows where each row has the following format:
 * [Node1] [Node2] [Sim]
 * [Node1],[Node2] are strings used to identify objects/nodes
 * [Sim] is a double value in range [0,inf]
 *
 */
class RowSimInput: public yskInput::StreamInput {
public:
	RowSimInput(ysk::ClusterEditingInstance* inst, double threshold);
	bool parseInput(std::istream &is);

private:
	double _threshold;
};

} /* namespace yskInput */

#endif /* SRC_INPUT_ROWSIMINPUT_H_ */
