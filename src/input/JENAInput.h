#ifndef SRC_INPUT_JENAINPUT_H
#define SRC_INPUT_JENAINPUT_H

#include "StreamInput.h"

#include <stdio.h>

namespace yskInput{

	/**
	 * This class describes the JENA input format and handles parsing of it.
	 * This is equivalent to the TransClustSimMatrixDataSetFormat
	 *
	 * Description:
	 * The first line contains an integer describing how many nodes the graph has
	 * The following |nodes| lines contain the ids (node names)
	 * The following lines can be described as a upper triangular distance matrix so:
	 * The following |nodes| lines contain each nodes edge-weight to each other respective node, separated by whitespace
	 *
	 * An easy example:
	 * 4 (LINE 1) There are 4 nodes
	 * A (LINE 2) The first node is called "A"
	 * B (LINE 3) The second node is called "B"
	 * C (LINE 4) The third node is called "C"
	 * D (LINE 5) The fourth node is called "D"
	 *
	 * 2 1 0 (LINE 6) A -> B has weight 2, A -> C has weight 1, A -> D has weight 0
	 *   1 0 (LINE 7) B -> C has weight 1, B -> D has weight 0
	 *     1 (LINE 8) C -> D has weight 1
	 */
	class JENAInput : public StreamInput{

	public:
		/**
		 * Default constructor
		 * @param inst The ClusterEditingInstance into which the input is to be parsed (see ClusterEditingInput.h)
		 */
		JENAInput(ysk::ClusterEditingInstance* inst) : StreamInput(inst){};


		/**
		 * Parses the input (see StreamInput.h)
		 * @param is The input stream from which the instance is to be parsed
		 * @return Returns true if successful, false otherwise
		 * TODO: Thinking about a coherent way of exception handling, maybe throw exception and return void
		 */
		bool parseInput(std::istream &is);

	};

}


#endif /* SRC_INPUT_JENAINPUT_H */
