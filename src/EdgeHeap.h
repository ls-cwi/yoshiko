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
  /**
   * Ensures that the heap structure of the given heap stays intact after the icf/icp value of an edge has been modified. 
   * Provided are the id of the modified edge, new and old value, an index (which maps edge ids to their position in the heap)
   * and a score vector (which maps an edge id to either its icf or icp).
   */
  void updateHeap(std::vector<LightCompleteGraph::EdgeId>& heap, const LightCompleteGraph::EdgeId e, const LightCompleteGraph::EdgeWeight newW, 
		  const LightCompleteGraph::EdgeWeight oldW, std::vector<LightCompleteGraph::EdgeId>& index, const std::vector<LightCompleteGraph::EdgeWeight>& score);
  
  LightCompleteGraph& graph;
  std::vector<LightCompleteGraph::EdgeWeight> icf;		// edge id -> icf of edge
  std::vector<LightCompleteGraph::EdgeWeight> icp;		// edge id -> icp of edge
  std::vector<LightCompleteGraph::EdgeId> forb_rank2edge;	// max-heap over edge ids, sorted by icf
  std::vector<LightCompleteGraph::EdgeId> perm_rank2edge;	// max-heap over edge ids, sorted by icp
  std::vector<LightCompleteGraph::EdgeId> edge2forb_rank;	// edge id -> position in icf-heap
  std::vector<LightCompleteGraph::EdgeId> edge2perm_rank;	// edge id -> position in icp-heap
};

} // namespace ysk;

#endif // EDGEHEAP_H
