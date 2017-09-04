//
//  clusterEditingSolutions.h
//  yoshiko
//
//  Created by Gunnar Klau on 15-06-12.
//  Copyright (c) 2012 Centrum Wiskunde & Informatica (CWI). All rights reserved.
//

#ifndef CLUSTEREDITINGSOLUTION_H
#define CLUSTEREDITINGSOLUTION_H

#include "Helpers.h"
#include "ClusterEditingInstance.h"
#include "WorkingCopyInstance.h"
#include "Globals.h"

#include <ilconcert/iloalg.h>

#include <lemon/list_graph.h>
#include <lemon/connectivity.h>

#include <vector>

namespace ysk {

///
class ClusterEditingSolutions {
public:

  ClusterEditingSolutions(){
	  _totalCost = 0;
	  _timedOut = false;
  }

  size_t getNumberOfClusters(size_t i) const;
  
  std::vector<int>& getCluster(size_t i, size_t k);
  
  std::vector<std::vector<int> >& getSolution(size_t i);
  
  void resize(long numberOfSolutions);
  

  //Setter/Getter methods

  void setSolution(int k,
                   const IloNumArray &x_vals,
                   const ClusterEditingInstance &inst);
  
  void setSolution(int k,
                   const ClusterEditingInstance& inst);
  
  void setSolution(int k,
                   const WorkingCopyInstance& inst);
  
  size_t getNumberOfSolutions() const;
  
  double getTotalCost() const;

  void setTotalCost(double totalCost);


  void setTimedOut(bool timedOut);
  bool isTimedOut();
  
private:
  // the partitions (multiple because of possible multiple optimal solutions)
  std::vector<std::vector<std::vector<int> > > _solutions;
  double _totalCost;
  /**
   * internal variable that should be set if this set of solutions was acquired through a timed-out algorithm run
   */
  bool _timedOut;
};
  
} // namespace ysk

#endif /* CLUSTEREDITINGSOLUTION_H */
