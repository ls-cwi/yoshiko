#ifndef EDGEHEAP_H
#define EDGEHEAP_H

#include <vector>
#include "LightCompleteGraph.h"

namespace ysk {

class EdgeHeap {
public:

/**
 * Constructs a new instance using the provided graph to precompute all icf and icp values.
 */
EdgeHeap(LightCompleteGraph& param_graph);

/**
 * Returns the edge with the highest icf.
 */
LightCompleteGraph::Edge getMaxIcfEdge() const;
/**
 * Returns the edge with the highest icp.
 */
LightCompleteGraph::Edge getMaxIcpEdge() const;
/**
 * Returns the icf of the provided edge.
 */
LightCompleteGraph::EdgeWeight getIcf(const LightCompleteGraph::Edge e) const;
/**
 * Returns the icp of the provided edge.
 */
LightCompleteGraph::EdgeWeight getIcp(const LightCompleteGraph::Edge e) const;
/**
 * Sets the icf for the provided edge to the new value.
 */
void updateIcf(const LightCompleteGraph::Edge e, const LightCompleteGraph::EdgeWeight w);
/**
 * Sets the icp for the provided edge to the new value.
 */
void updateIcp(const LightCompleteGraph::Edge e, const LightCompleteGraph::EdgeWeight w);
/**
 * Removes the specified edge.
 */
void removeEdge(const LightCompleteGraph::Edge e);
  /**
   * Computes the induced cost for the the triple uvw, if uv is set to forbidden
   */
  LightCompleteGraph::EdgeWeight getIcf(const LightCompleteGraph::EdgeWeight uv, const LightCompleteGraph::EdgeWeight uw, const LightCompleteGraph::EdgeWeight vw);
  /**
   * Computes the induced cost for the the triple uvw, if uv is set to permanent
   */
  LightCompleteGraph::EdgeWeight getIcp(const LightCompleteGraph::EdgeWeight uv, const LightCompleteGraph::EdgeWeight uw, const LightCompleteGraph::EdgeWeight vw);

private:
  void initInducedCosts();
  /**
   * Returns the edge id with the highest cost inside the provided vector.
   */
  LightCompleteGraph::Edge getMaxEdge(const std::vector<LightCompleteGraph::EdgeWeight>& vec) const;
  
  LightCompleteGraph& graph;
  std::vector<LightCompleteGraph::EdgeWeight> icf;
  std::vector<LightCompleteGraph::EdgeWeight> icp;
};

} // namespace ysk;

#endif // EDGEHEAP_H
