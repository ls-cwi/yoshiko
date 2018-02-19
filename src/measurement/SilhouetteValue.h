#ifndef SRC_MEASUREMENT_SILHOUETTEVALUE_H_
#define SRC_MEASUREMENT_SILHOUETTEVALUE_H_

#include "ClusterEditingInstance.h"

#include <lemon/full_graph.h>
#include <lemon/adaptors.h>
#include <lemon/connectivity.h>

namespace ysk{
	/**
	 *  Implementation of the Silhouette Value as described in
	 *  Peter J. Rousseeuw. “Silhouettes: A graphical aid to the interpretation and validation of cluster analysis”.
	 *	In: Journal  of  Computational  and Applied  Mathematics
	 *	20.Supplement C (1987), pp. 53–65.
	 *  issn: 0377-0427.
	 *	doi: https://doi.org/10.1016/0377-0427(87)90125-7
	 *	url: http://www.sciencedirect.com/science/article/pii/0377042787901257
	 */
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
