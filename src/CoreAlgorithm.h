#ifndef SRC_COREALGORITHM_H
#define SRC_COREALGORITHM_H

#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>
#include <set>

#include "ClusterEditingSolutions.h"
#include "ClusterEditingReduction.h"
#include "CplexInformer.h"
#include "ILPSolver.h"
#include "YParameterSet.h"

namespace ysk {

	class CoreAlgorithm{

		public:

		CoreAlgorithm(
				ClusterEditingInstance* instance,
				YParameterSet parameter
		)
		:_instance(instance)
		,_result(new ClusterEditingSolutions)
		,_parameter(parameter)
		,_useInformer(false)
		,_solverInitialized(false)
		{};

		ClusterEditingSolutions* run();

		void cancel();

		void registerCplexInformer(yskLib::CplexInformer* informer);


		private:
			ClusterEditingInstance* _instance;
			ClusterEditingSolutions* _result;

			YParameterSet _parameter;

			yskLib::CplexInformer* _informer;
			ILPSolver _solver;

			bool _useInformer;
			bool _solverInitialized;

	};

	//replace reduced cluster by single nodes
	void expandSolutions(ClusterEditingInstance& cei,
			ClusterEditingSolutions& ces,
			std::vector<std::vector<std::vector<int> > >& partitions);

	void mergeSolutions(size_t i, size_t& k,
			std::vector<std::vector<int> >& partition,
			ClusterEditingSolutions& solutions,
			std::vector<std::vector<std::vector<std::vector<int> > > >& instances);

}

#endif /* SRC_COREALGORITHM_H */