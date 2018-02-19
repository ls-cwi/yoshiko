#ifndef SRC_LIBRARYINTERFACE_H
#define SRC_LIBRARYINTERFACE_H

#include <string>

#define CPXSIZE_BITS 64 //<<< Why? Good question!

#include "Globals.h"
#include "config.h"

#include "input/LibraryInput.h"

#include "ClusterEditingSolutions.h"
#include "CoreAlgorithm.h"

/**
 * Namespace for external access
 */
namespace yskLib{
		/**
		 * Returns the current version, useful for version checking.
		 * @return The current version as string (git hash)
		 */
		std::string getVersionString();

		/**
		 * Sets the global time limit for the ILP part of the software
		 * @param limit The timeout threshold in seconds
		 */
		void setTimeLimit(int limit);

		/**
		 * Sets the global thread limit for the ILP part of the software
		 * @param limit The number of threads that can be used
		 */
		void setThreadLimit(int limit);

		/**
		 * Sets the verbosity
		 * @param level The level, corresponding to a verbosity level, will be ignored if invalid
		 */
		void setVerbosity(int level);

		ysk::CoreAlgorithm* getRun(
				yskInput::LibraryInput* input,
				int nrOptimalSolutions,
				std::string rulesBitMask,
				double multiplicativeFactor,
				bool useHeuristic,
				bool separatePartitionCuts,
				bool separateTriangles,
				int targetClusterCount
		);
}

#endif /* SRC_LIBRARYINTERFACE_H */
