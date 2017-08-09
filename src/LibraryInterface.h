/*
 * LibraryInterface.h
 *
 *  Created on: Aug 6, 2017
 *      Author: Philipp Spohr
 */

#ifndef SRC_LIBRARYINTERFACE_H_
#define SRC_LIBRARYINTERFACE_H_

#include "config.h"
#include "ClusterEditingInstance.h"

#include <string>

/**
 * Namespace for external access
 */
namespace yskLib{
		/**
		 * Returns the current version, useful for version checking.
		 * @return The current version as string (git hash)
		 */
		std::string getVersionString();

		ysk::ClusterEditingInstance* createNewProblemInstance();
}

#endif /* SRC_LIBRARYINTERFACE_H_ */
