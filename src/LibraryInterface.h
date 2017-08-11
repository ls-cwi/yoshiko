/*
 * LibraryInterface.h
 *
 *  Created on: Aug 6, 2017
 *      Author: Philipp Spohr
 */

#ifndef SRC_LIBRARYINTERFACE_H_
#define SRC_LIBRARYINTERFACE_H_

#include "config.h"

#include <string>
#include "input/LibraryInput.h"

/**
 * Namespace for external access
 */
namespace yskLib{
		/**
		 * Returns the current version, useful for version checking.
		 * @return The current version as string (git hash)
		 */
		std::string getVersionString();
}

#endif /* SRC_LIBRARYINTERFACE_H_ */
