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

using namespace std;


namespace yskInput{

/**
 * Class that serves as an interface for the various input formats that can be provided to the yoshiko library.
 * Inheritance of this class should provide a mean of validating and providing input in a given format.
 */
class ClusterEditingInput{
	public:

		ClusterEditingInput(ysk::ClusterEditingInstance* inst){
			_instance = inst;
		}

		/**
		 * Returns the ClusterEditingInstance that is represented by this input (after processing/parsing)
		 * @return The pointer referencing the ClusterEditingInstance
		 */
		ysk::ClusterEditingInstance* getProblemInstance();

	protected:


		/**
		 * The ClusterEditingInstance that is adjusted according to the input provided and can eventually be retrieved to apply the algorithm
		 */
		ysk::ClusterEditingInstance* _instance;
};

}

#endif /* CLUSTEREDITINGINPUT_H */
