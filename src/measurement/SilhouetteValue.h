/*
 * SilhouetteValue.h
 *
 *  Created on: Jan 15, 2018
 *      Author: philipp
 */

#ifndef SRC_MEASUREMENT_SILHOUETTEVALUE_H_
#define SRC_MEASUREMENT_SILHOUETTEVALUE_H_

#include "ClusterEditingInstance.h"

#include <lemon/full_graph.h>
#include <lemon/adaptors.h>
#include <lemon/connectivity.h>

namespace ysk{
	class SilhouetteValue {
	public:
		SilhouetteValue(ClusterEditingInstance* instance,std::vector<std::vector<int>>& solution):
			_instance(instance),
			_solution(solution)
		{};

		virtual ~SilhouetteValue();
		double getValue();
	private:
		ClusterEditingInstance* _instance;
		std::vector<std::vector<int>> _solution;

	};
}


#endif /* SRC_MEASUREMENT_SILHOUETTEVALUE_H_ */
