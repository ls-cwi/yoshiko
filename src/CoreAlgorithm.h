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
#include "KClustifier.h"
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
		,_informer(nullptr)
		,_solver(nullptr)
		{};

		ClusterEditingSolutions* run();

		void registerCplexInformer(yskLib::CplexInformer* informer);

		/**
		 * Attempts a "clean" interrupt of the solving process by stopping CPLEX and setting a kill-flag which is checked throughout the process
		 */
		void cancel();

		private:
			ClusterEditingInstance* _instance;
			ClusterEditingSolutions* _result;

			YParameterSet _parameter;

			yskLib::CplexInformer* _informer;

			ILPSolver* _solver;

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
