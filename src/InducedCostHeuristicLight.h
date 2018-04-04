#ifndef INDUCEDCOSTHEURISTICLIGHT_H
#define INDUCEDCOSTHEURISTICLIGHT_H

#include "LightCompleteGraph.h"
#include "EdgeHeap.h"
#include "ClusterEditingSolutionLight.h"

namespace ysk {

class InducedCostHeuristicLight {
  

public:
  InducedCostHeuristicLight(LightCompleteGraph& param_graph);
  ClusterEditingSolutionLight solve();

private:    
  void init();
  void setForbidden(const LightCompleteGraph::Edge e);
  void setPermanent(const LightCompleteGraph::Edge e);
  /**
   * Updates icf and icp for the edge uw under the assumption that edge uv will be set to forbidden.
   */
  void updateTripleForbiddenUW(const LightCompleteGraph::NodeId u, const LightCompleteGraph::NodeId v, const LightCompleteGraph::NodeId w);
  /**
   * Updates icf and icp for the edge uw under the assumption that edge uv will be set to permanent.
   */
  void updateTriplePermanentUW(const LightCompleteGraph::NodeId u, const LightCompleteGraph::NodeId v, const LightCompleteGraph::NodeId w);
  
  LightCompleteGraph graph;
  EdgeHeap edges;
  LightCompleteGraph::EdgeWeight totalCost;
};

} // namespace ysk

#endif // INDUCEDCOSTHEURISTICLIGHT_H
