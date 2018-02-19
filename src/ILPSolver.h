#ifndef ILPSOLVER_H
#define ILPSOLVER_H

// yoshiko stuff
#include "ClusterEditingInstance.h"
#include "CplexInformer.h"
#include "WorkingCopyInstance.h"
#include "ClusterEditingSolutions.h"
#include "Globals.h"

// ILOG stuff
#include <ilcplex/ilocplex.h>
#include <ilcplex/ilocplexi.h>
#include <ilconcert/iloalg.h>

// lemon stuff
#include <lemon/time_measure.h>
#include <lemon/list_graph.h>
#include <lemon/connectivity.h>

namespace ysk {

class ILPSolver {
public:

	// constructor
	ILPSolver(bool st, bool sp, int num_opt_sol)
	: _sep_triangles(st)
	, _sep_partition_cuts(sp)
	, _num_opt_sol(num_opt_sol)
	,_useKCluster(false)
	,_clusterCount(0)
	,_cplexInitialized(false)
	,_informer(nullptr)
	{};

	// alternative constructor for k-cluster variant
	ILPSolver(bool st, bool sp, int num_opt_sol, int clusterCount)
	: _sep_triangles(st)
	, _sep_partition_cuts(sp)
	, _num_opt_sol(num_opt_sol)
	,_useKCluster(true)
	,_clusterCount(clusterCount)
	,_cplexInitialized(false)
	,_informer(nullptr)
	{};


	long solve(const ClusterEditingInstance& i,
			ClusterEditingSolutions& s,
			SolutionFlags& flags);

	void terminate();

	void registerInformer(yskLib::CplexInformer* informer);

private:
	bool _sep_triangles;
	bool _sep_partition_cuts;
	int _num_opt_sol;

	//K-Cluster variables
	bool _useKCluster;
	int _clusterCount;

	IloCplex::Aborter _aborter;
	bool _cplexInitialized;

	yskLib::CplexInformer* _informer;
};

} // namespace ysk

#endif /* ILPSOLVER_H */
