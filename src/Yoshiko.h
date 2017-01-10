//
//  Yoshiko.h
//  yoshiko
//
//  Created by Gunnar Klau on 15-06-12.
//  Copyright (c) 2012 Centrum Wiskunde & Informatica (CWI). All rights reserved.
//

#ifndef YOSHIKO_H
#define YOSHIKO_H

// yoshiko stuff
#include "ClusterEditingInstance.h"
#include "WorkingCopyInstance.h"
#include "ClusterEditingSolutions.h"
#include "Globals.h"

// ILOG stuff
#include <ilcplex/ilocplex.h>
#include <ilconcert/iloalg.h>

// lemon stuff
#include <lemon/time_measure.h>
#include <lemon/list_graph.h>
#include <lemon/connectivity.h>

namespace ysk {
  
///
/// \brief yoshiko, the cluster editing solver based on integer linear programming
/// \author Gunnar W. Klau
///

class Yoshiko {
public:
  // constructor
  Yoshiko(bool st, bool sp, int num_opt_sol)
    : _sep_triangles(st)
    , _sep_partition_cuts(sp)
    , _num_opt_sol(num_opt_sol)
    {
    };
  
  long solve(const ClusterEditingInstance& i,
             ClusterEditingSolutions& s);
  
private:
  bool _sep_triangles;
  bool _sep_partition_cuts;
  int _num_opt_sol;
};
  
} // namespace ysk

#endif /* YOSHIKO_H */
