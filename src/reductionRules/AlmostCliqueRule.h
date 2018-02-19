#ifndef ALMOSTCLIQUERULE_H
#define	ALMOSTCLIQUERULE_H

#include <lemon/full_graph.h>
#include <lemon/adaptors.h>
#include <lemon/bin_heap.h>
#include <lemon/hao_orlin.h>

#include <cmath>
#include <limits>

#include "WorkingCopyInstance.h"
#include "FPTReductionRule.h"

namespace ysk {

class AlmostCliqueRule : public FPTReductionRule {
public:
  typedef lemon::BinHeap<double, WorkingCopyGraph::NodeMap<int>, std::greater<double> > MaxHeap;
  
  AlmostCliqueRule(WorkingCopyInstance& inst,
                   bool conserveMultipleSolutions)
    : FPTReductionRule(inst, "Almost Clique Rule", conserveMultipleSolutions)
    , _connectivityCrossRef(inst.getGraph(), -1)
    , _connectivity(_connectivityCrossRef)
  {
  }
  
  void apply();
  
private:
  void init();
  
  void merge(WorkingCopyGraph::NodeMap<bool>& candidate);
  
  WorkingCopyGraph::Node findNodeMaxConnectivity(WorkingCopyGraph::Node& lastAdded,
                                                 WorkingCopyGraph::NodeMap<bool>& candidate,
                                                 WorkingCopyGraph::NodeMap<double>& connectivityCandidate);
  double computeMinCutValue(WorkingCopyGraph::NodeMap<bool>& candidate);
  double computeCost(WorkingCopyGraph::NodeMap<bool>& candidate);
  
  WorkingCopyGraph::NodeMap<int> _connectivityCrossRef;
  MaxHeap _connectivity;
};
  
} // namespace ysk

#endif	/* ALMOSTCLIQUERULE_H */
