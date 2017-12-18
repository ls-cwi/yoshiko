//
//  ParameterizedInstance.h
//  lemon-first-steps
//
//  Created by Emanuel Laude on 22.10.12.
//  Copyright (c) 2012 Emanuel Laude. All rights reserved.
//

//TODO: Enable cancelling of heuristic

#ifndef PARAMETERIZEDINSTANCE_H
#define PARAMETERIZEDINSTANCE_H

#include <iostream>
#include <list>
#include <cmath>
#include <set>
#include <limits>

#include "WorkingCopyInstance.h"

#include <lemon/bin_heap.h>

namespace ysk {

class ParameterizedInstance {
public:
  typedef lemon::BinHeap<double, WorkingCopyGraph::NodeMap<int>, std::greater<double> > MaxHeap;

  ParameterizedInstance(WorkingCopyInstance& inst, double parameter)
    : _instance(inst)
    , _parameter(parameter)
    , _lbValue(0.0)
    , _lbDecrease(inst.getGraph(), 0.0)
    , _conflictTriple(inst.getGraph())
    , _nodesLowerBound(inst.getGraph(), 0.0)
    , _edgesLowerBound(inst.getGraph(), 0.0)
    , _icfCrossRef(inst.getGraph())
    , _icf(inst.getGraph())
    , _icpCrossRef(inst.getGraph())
    , _icp(inst.getGraph())
  {
    for(WorkingCopyGraph::NodeIt u(_instance.getGraph()); u != lemon::INVALID; ++u) {
      _icpCrossRef[u] = new WorkingCopyGraph::NodeMap<int>(_instance.getGraph(), -1);
      _icp[u] = new MaxHeap(*_icpCrossRef[u]);

      _icfCrossRef[u] = new WorkingCopyGraph::NodeMap<int>(_instance.getGraph(), -1);
      _icf[u] = new MaxHeap(*_icfCrossRef[u]);
    }
  }

  ParameterizedInstance(WorkingCopyInstance& inst,
                        WorkingCopyGraph::NodeMap<WorkingCopyGraph::Node>& aref,
                        WorkingCopyGraph::NodeMap<WorkingCopyGraph::Node>& bref,
                        ParameterizedInstance& paramInst)
    : _instance(inst)
    , _parameter(std::numeric_limits<double>::signaling_NaN())
    , _lbValue(0.0)
    , _lbDecrease(inst.getGraph(), 0.0)
    , _conflictTriple(inst.getGraph())
    , _nodesLowerBound(inst.getGraph(), 0.0)
    , _edgesLowerBound(inst.getGraph(), 0.0)
    , _icfCrossRef(inst.getGraph()), _icf(inst.getGraph())
    , _icpCrossRef(inst.getGraph()), _icp(inst.getGraph())
  {
    for(WorkingCopyGraph::NodeIt u(_instance.getGraph()); u != lemon::INVALID; ++u) {
      _icpCrossRef[u] = new WorkingCopyGraph::NodeMap<int>(_instance.getGraph(), -1);
      _icp[u] = new MaxHeap(*_icpCrossRef[u]);

      _icfCrossRef[u] = new WorkingCopyGraph::NodeMap<int>(_instance.getGraph(), -1);
      _icf[u] = new MaxHeap(*_icfCrossRef[u]);
    }

    init(aref, bref, paramInst);
  }

  virtual ~ParameterizedInstance();

  void init();
  double getParameter();
  void setParameter(double parameter);
  void setForbidden(const WorkingCopyGraph::Edge& uv);
  void setPermanent(const WorkingCopyGraph::Edge& uv);
  double getICP(const WorkingCopyGraph::Edge& uv);
  double getICF(const WorkingCopyGraph::Edge& uv);
  void setICP(const WorkingCopyGraph::Edge& uv, double value);
  void setICF(const WorkingCopyGraph::Edge& uv, double value);
  double getLowerBound();
  WorkingCopyInstance& getWorkingCopyInstance();

  std::pair<WorkingCopyGraph::Edge, double> computeMax();

private:
  void init(WorkingCopyGraph::NodeMap<WorkingCopyGraph::Node>& aref,
            WorkingCopyGraph::NodeMap<WorkingCopyGraph::Node>& bref,
            ParameterizedInstance& paramInst);
  void computeInducedCosts(const WorkingCopyGraph::Edge& uv,
                           double& icp,
                           double& icf);
  double getLBDecrease(const WorkingCopyGraph::Edge& uv);
  void updateLBDecrease(const WorkingCopyGraph::Edge& uv);
  void computeLowerBound();
  void updateLowerBound(const WorkingCopyGraph::Edge& uv,
                        double value);
  void updateLowerBoundBeforeSetForbidden(const WorkingCopyGraph::Edge& uv,
                                          std::vector<WorkingCopyGraph::Node*>& cts);
  void updateLowerBoundBeforeSetPermanent(const WorkingCopyGraph::Edge& uv,
                                          const WorkingCopyGraph::Node& proxy);
  void computeLowerBound(const WorkingCopyGraph::Edge &uv,
                         lemon::BinHeap<double, WorkingCopyGraph::EdgeMap<int>,
                         std::greater<double> > &edgesOrderByWeight,
                         WorkingCopyGraph::EdgeMap<double>& weight,
                         WorkingCopyGraph::EdgeMap<bool>& candidate,
                         std::vector<WorkingCopyGraph::Node*>& cts);
  void beforeSetForbidden(const WorkingCopyGraph::Edge& uv);
  void beforeSetPermanent(const WorkingCopyGraph::Edge& uv,
                          const WorkingCopyGraph::Node& proxy);
  void afterSetPermanent(const WorkingCopyGraph::Edge& uv,
                         WorkingCopyGraph::Node& proxy);
  void updateICP(const WorkingCopyGraph::Edge& uv,
                 double value);
  void updateICF(const WorkingCopyGraph::Edge& uv,
                 double value);
  void removeICP(const WorkingCopyGraph::Edge& uv);
  void removeICF(const WorkingCopyGraph::Edge& uv);
  std::pair<WorkingCopyGraph::Node, WorkingCopyGraph::Node> getNodesOrderById(const WorkingCopyGraph::Edge& uv);

  WorkingCopyInstance& _instance;
  double _parameter;
  double _lbValue;
  WorkingCopyGraph::EdgeMap<double> _lbDecrease;
  WorkingCopyGraph::EdgeMap<WorkingCopyGraph::Node* > _conflictTriple;
  std::vector<WorkingCopyGraph::Node* > _conflictTriples;

  WorkingCopyGraph::NodeMap<double> _nodesLowerBound;
  WorkingCopyGraph::EdgeMap<double> _edgesLowerBound;

  WorkingCopyGraph::NodeMap<WorkingCopyGraph::NodeMap<int>* > _icfCrossRef;
  WorkingCopyGraph::NodeMap<MaxHeap* > _icf;
  WorkingCopyGraph::NodeMap<WorkingCopyGraph::NodeMap<int>* > _icpCrossRef;
  WorkingCopyGraph::NodeMap<MaxHeap* > _icp;
};

} // namespace ysk

#endif /* PARAMETERIZEDINSTANCE_H */
