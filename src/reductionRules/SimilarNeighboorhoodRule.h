/*
 * File:   SimilarNeighboorhoodRule.h
 * Author: emanuellaude
 *
 * Created on 6. September 2012, 14:02
 */

#ifndef SIMILARNEIGHBOORHOODRULE_H
#define	SIMILARNEIGHBOORHOODRULE_H

#include <lemon/adaptors.h>
#include <lemon/full_graph.h>
#include <lemon/lgf_writer.h>
#include <lemon/connectivity.h>
#include <lemon/bin_heap.h>

#include <vector>
#include <limits>
#include <cmath>

#include "WorkingCopyInstance.h"
#include "FPTReductionRule.h"
#include "DynProgTable.h"
#include "Delta.h"

namespace ysk {

class SimilarNeighborhoodRule : public FPTReductionRule {
  typedef lemon::BinHeap<double, WorkingCopyGraph::EdgeMap<int>, std::less<double> > MinHeap;
  
public:
  SimilarNeighborhoodRule(WorkingCopyInstance& inst,
                          double c,
                          bool conserveMultipleSolutions)
    : FPTReductionRule(inst, "Similar Neighborhood Rule", conserveMultipleSolutions)
    , _c(c)
    , _delta(inst.getGraph())
    , _upperBoundCrossRef(inst.getGraph(), -1)
    , _upperBound(_upperBoundCrossRef)
  {
  }
  
  void apply();
  
private:
  void init();
  
  double computeBounds(WorkingCopyGraph::Edge uv);
  
  void setPermanent(WorkingCopyGraph::Edge uv);
  
  void setPermanent(std::vector<std::pair<WorkingCopyGraph::Edge, double> >* &skip);
  
  void beforeSetPermanent(WorkingCopyGraph::Edge uv);
  
  void afterSetPermanent(WorkingCopyGraph::Node proxy);
  
  void updateUpperBound(WorkingCopyGraph::Edge uv, double value);
  
  double computeMaximum(WorkingCopyGraph::Edge uv);
  
  double computeLowerBound(WorkingCopyGraph::Edge uv);
  
  double computeDelta(WorkingCopyGraph::NodeMap<bool>& neighborhood,
                      WorkingCopyGraph::NodeMap<bool>& neighborhoodOpp,
                      WorkingCopyGraph::Node& node);
  
  double _c;
  WorkingCopyGraph::EdgeMap<Delta> _delta;
  WorkingCopyGraph::EdgeMap<int> _upperBoundCrossRef;
  MinHeap _upperBound;
};

} // namespace ysk
  
#endif	/* SIMILARNEIGHBOORHOODRULE_H */
