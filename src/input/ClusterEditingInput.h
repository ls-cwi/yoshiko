//  ClusterEditingInput.h

#ifndef CLUSTEREDITINGINPUT_H
#define CLUSTEREDITINGINPUT_H

#include "ClusterEditingInstance.h"

#include <lemon/adaptors.h>
#include <lemon/lgf_writer.h>
#include <lemon/full_graph.h>

#include <vector>
#include <cmath>
#include <string>
#include <limits>
#include <map>

#include "Helpers.h"

namespace yskInput{

/**
 * Virtual class that serves as an interface for the various input formats that can be provided to the yoshiko library.
 * Inheritance of this class should provide a mean of validating and parsing input in a given format.
 */
class ClusterEditingInput{
	public:

		/**
		 * Parses the input stream treating it as the input format represented by the respective class implementing this method
		 * @param is The input stream which is to be parsed
		 * @return A ysk::ClusterEditingInstance object pointer referencing the parsed input
		 */
		virtual ysk::ClusterEditingInstance* parseInput(std::istream &is) = 0;

		/**
		 * Validates if the input satisfies the input format represented by the class implementing this method
		 * @param is The input stream which is to be validated
		 * @return True if valid, false otherwise
		 */
		virtual bool validateInput(std::istream &is) = 0;

		/**
		 * Destructor
		 */
		virtual ~ClusterEditingInput(){};
};

}

#endif /* CLUSTEREDITINGINPUT_H */
