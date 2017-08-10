/*
 * StreamInput.h
 *
 *  Created on: Aug 10, 2017
 *      Author: philipp
 */

#ifndef SRC_INPUT_STREAMINPUT_H_
#define SRC_INPUT_STREAMINPUT_H_

#include "ClusterEditingInput.h"

namespace yskInput{
	
	/**
	 * Virtual class that serves as a template for any input format that is provided as an actual input stream (usually by means of an input file)
	 */
	class StreamInput : public ClusterEditingInput{
		public:
	
			/**
			 * Parses the input stream treating it as the input format represented by the respective class implementing this method
			 * @param is The input stream which is to be parsed
			 */
			virtual void parseInput(std::istream &is) = 0;
	
			/**
			 * Validates if the input satisfies the input format represented by the class implementing this method
			 * @param is The input stream which is to be validated
			 * @return True if valid, false otherwise
			 */
			virtual bool validateInput(std::istream &is) = 0;
	
			virtual ~StreamInput(){};
	
	
	};

}

#endif /* SRC_INPUT_STREAMINPUT_H_ */
