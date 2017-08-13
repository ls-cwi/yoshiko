/*
 * LibraryInterface.h
 *
 *  Created on: Aug 6, 2017
 *      Author: Philipp Spohr
 */

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

		ysk::ClusterEditingSolutions* processLibraryInput(yskInput::LibraryInput* libIn);

		/**
		 * Sets the global time limit for the ILP part of the software
		 * @param limit The timeout threshold in seconds
		 */
		void setTimeLimit(int limit);
}

#endif /* SRC_LIBRARYINTERFACE_H */
