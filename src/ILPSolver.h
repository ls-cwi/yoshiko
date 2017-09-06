//  ILPSolver.h
//
//  Created by Gunnar Klau on 15-06-12.
//  Copyright (c) 2012 Centrum Wiskunde & Informatica (CWI). All rights reserved.
//  Contributions: Emanuel Laude, Philipp Spohr

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
	ILPSolver()
	: _sep_triangles(false)
	, _sep_partition_cuts(false)
	, _num_opt_sol(1)
	,_cplexInitialized(false)
	,_useInformer(false)
	{};

  // constructor
	ILPSolver(bool st, bool sp, int num_opt_sol)
    : _sep_triangles(st)
    , _sep_partition_cuts(sp)
    , _num_opt_sol(num_opt_sol)
	,_cplexInitialized(false)
	,_useInformer(false)
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

  IloCplex::Aborter _aborter;
  bool _cplexInitialized;

  yskLib::CplexInformer* _informer;
  bool _useInformer;
};
  
} // namespace ysk

#endif /* ILPSOLVER_H */
