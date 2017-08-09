//  ClusterEditingInput.h

#ifndef CLUSTEREDITINGINPUT_H
#define CLUSTEREDITINGINPUT_H

#include "ClusterEditingInstance.h"


namespace yskInput{

/**
 * Pure virtual class that serves as an interface for the various input formats that can be provided to the yoshiko library.
 * Inheritance of this class should provide a mean of validating and parsing input in a given format.
 */
class ClusterEditingInput{
	public:

		/**
		 * Parses the input stream treating it as the input format represented by the respective class implementing this method
		 * @param is The input stream which is to be parsed
		 * @return A ysk::ClusterEditingInstance object pointer referencing the parsed input
		 */
		virtual ysk::ClusterEditingInstance* parseInput(std::istream &is);

		/**
		 * Validates if the input satisfies the input format represented by the class implementing this method
		 * @param is The input stream which is to be validated
		 * @return True if valid, false otherwise
		 */
		virtual bool validateInput(std::istream &is);

		/**
		 * Destructor
		 */
		virtual ~ClusterEditingInput();

	private:
		//Note: Constructor is hidden as this class serves purely as an interface and should never exist as an instance
		ClusterEditingInput();
};

}

#endif /* CLUSTEREDITINGINPUT_H */
