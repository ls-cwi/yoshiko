#include "EdgeHeap.h"

namespace ysk {
  
using Edge = LightCompleteGraph::Edge;
using EdgeWeight = LightCompleteGraph::EdgeWeight;
using EdgeId = LightCompleteGraph::EdgeId;
using NodeId = LightCompleteGraph::NodeId;

EdgeHeap::EdgeHeap(LightCompleteGraph& param_graph) :
  graph(param_graph),
  icf(param_graph.numEdges(), 0.0),
  icp(param_graph.numEdges(), 0.0)
{
  initInducedCosts();
}

void EdgeHeap::initInducedCosts() {
  // compute array: edge -> icf/icp
  for (NodeId u = 0; u < graph.numNodes(); u++) {
    for (NodeId v = u + 1; v < graph.numNodes(); v++) {
      // iterate over all edges uv
      Edge uv(u,v);
      EdgeWeight w_uv = graph.getWeight(uv);
      EdgeId id = uv.id();
      
      // costs for the edge uv itself
      if (w_uv >= 0) {	
	icf[id] += w_uv;	// costs for removing uv
      } else {
	icp[id] += -w_uv;	// costs for adding uv
      }
      
      // look at all triangles uvw containing uv:
      for (NodeId w = 0; w < graph.numNodes(); w++) {
	if (u == w || v == w) {
	  continue;
	}
	Edge uw(u,w);
	Edge vw(v,w);
	EdgeWeight w_uw = graph.getWeight(uw);
	EdgeWeight w_vw = graph.getWeight(vw);
	icf[id] += getIcf(w_uv, w_uw, w_vw);
	icp[id] += getIcp(w_uv, w_uw, w_vw);
      }
    }
  }
}

Edge EdgeHeap::getMaxIcfEdge() const {
  return getMaxEdge(icf);
}

Edge EdgeHeap::getMaxIcpEdge() const {
  return getMaxEdge(icp);
}

EdgeWeight EdgeHeap::getIcf(const Edge e) const {
  return icf[e.id()];
}

EdgeWeight EdgeHeap::getIcp(const Edge e) const {
  return icp[e.id()];
}

void EdgeHeap::updateIcf(const Edge e, const EdgeWeight w) {
  if (icf[e.id()] >= 0) {
    icf[e.id()] = w;
  }
}

void EdgeHeap::updateIcp(const Edge e, const EdgeWeight w) {
  if (icp[e.id()] >= 0) {
    icp[e.id()] = w;
  }
}

void EdgeHeap::removeEdge(const Edge e) {
  icf[e.id()] = -1.0;
  icp[e.id()] = -1.0;
}

LightCompleteGraph::EdgeWeight EdgeHeap::getIcf(const EdgeWeight uv, const EdgeWeight uw, const EdgeWeight vw) {
  if (uw > 0 && vw > 0) {
    // if both other edges present, remove the cheapest of both
    return std::min(uw, vw); 
  } else {
    return 0;
  }
  
  // if both other edges present, remove the cheapest of both
//   return (uw > 0) * (vw > 0) * (std::min(uw, vw));
}

LightCompleteGraph::EdgeWeight EdgeHeap::getIcp(const EdgeWeight uv, const EdgeWeight uw, const EdgeWeight vw) {
  if (uw < 0 && vw > 0) {
    return std::min(vw, -uw); 	// either add uw or remove vw
  } else if (uw > 0 && vw < 0) {
    return std::min(-vw, uw); 	// either add vw or remove uw
  } else {
    return 0;
  }
  
  // either add vw or remove uw if different sign
//   int uw_l0 = uw < 0;
//   return (((uw_l0) + (vw < 0)) % 2) * ((uw_l0) * std::min(vw, -uw) + (1 - uw_l0) * std::min(-vw, uw));
}

Edge EdgeHeap::getMaxEdge(const std::vector<EdgeWeight>& vec) const {
  EdgeWeight maxIcf = -1.0;
  EdgeId maxId = LightCompleteGraph::InvalidEdgeId;
  for (unsigned int i = 0; i < vec.size(); i++) {
    if (vec[i] > maxIcf) {
      maxIcf = vec[i];
      maxId = i;
    }
  }
  if (maxId == LightCompleteGraph::InvalidEdgeId) {
    return graph.InvalidEdge;
  }
  NodeId u = std::ceil(std::sqrt(2*(maxId+1)+0.25) - 0.5);
  NodeId v = maxId - u * (u-1) / 2;
  return Edge(u, v);
}

} // namespace ysk