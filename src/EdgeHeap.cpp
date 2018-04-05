#include "EdgeHeap.h"

namespace ysk {
  
using Edge = LightCompleteGraph::Edge;
using EdgeWeight = LightCompleteGraph::EdgeWeight;
using EdgeId = LightCompleteGraph::EdgeId;
using NodeId = LightCompleteGraph::NodeId;

EdgeHeap::EdgeHeap(LightCompleteGraph& param_graph) :
  graph(param_graph),
  icf(param_graph.numEdges(), 0.0),
  icp(param_graph.numEdges(), 0.0),
  edge2forb_rank(param_graph.numEdges(), 0),
  edge2perm_rank(param_graph.numEdges(), 0)
{
  initInducedCosts();
}

void EdgeHeap::initInducedCosts() {
  std::cout<<"Compute induced cost."<<std::endl;
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
  
  // sort edges by icf and icp values
  for (EdgeId i = 0; i < graph.numEdges(); i++) {
    forb_rank2edge.push_back(i);
    perm_rank2edge.push_back(i);
  }
  std::sort(forb_rank2edge.begin(), forb_rank2edge.end(), [this] (NodeId& a, NodeId& b) { return icf[a] > icf[b]; });
  std::sort(perm_rank2edge.begin(), perm_rank2edge.end(), [this] (NodeId& a, NodeId& b) { return icp[a] > icp[b]; });
  
  // save index in sorted vectors for each edge
  for (EdgeId i = 0; i < graph.numEdges(); i++) {
    edge2forb_rank[forb_rank2edge[i]] = i;
    edge2perm_rank[perm_rank2edge[i]] = i;
  }

  for (unsigned int i = 0; i < forb_rank2edge.size()/2; i++) {
    if (icf[forb_rank2edge[i]] < icf[forb_rank2edge[2*i]]) {
      std::cout<<"Error in heap1"<<std::endl;
    }
    if (2*i+1 < forb_rank2edge.size() && icf[forb_rank2edge[i]] < icf[forb_rank2edge[2*i+1]]) {
      std::cout<<"Error in heap2"<<std::endl;
    }
  }
}

Edge EdgeHeap::getMaxIcfEdge() const {
  EdgeId ei = forb_rank2edge[0];
  NodeId u = std::ceil(std::sqrt(2*(ei+1)+0.25) - 0.5);
  NodeId v = ei - u * (u-1) / 2;
  return Edge(u, v);
//   return getMaxEdge(icf);
}

Edge EdgeHeap::getMaxIcpEdge() const {
  EdgeId ei = perm_rank2edge[0];
  NodeId u = std::ceil(std::sqrt(2*(ei+1)+0.25) - 0.5);
  NodeId v = ei - u * (u-1) / 2;
  return Edge(u, v);
//   return getMaxEdge(icp);
}

EdgeWeight EdgeHeap::getIcf(const Edge e) const {
  return icf[e.id()];
}

EdgeWeight EdgeHeap::getIcp(const Edge e) const {
  return icp[e.id()];
}

void EdgeHeap::updateIcf(const Edge e, const EdgeWeight w) {
  EdgeId id = e.id();
  EdgeWeight old = icf[id];
  if (old != w && icf[id] >= 0) {
    icf[id] = w;
    updateHeap(forb_rank2edge, id, w, old, edge2forb_rank, icf);
  }
}

void EdgeHeap::updateIcp(const Edge e, const EdgeWeight w) {
  EdgeId id = e.id();
  EdgeWeight old = icp[id];
  if (old != w && icp[id] >= 0) {
    icp[id] = w;
    updateHeap(perm_rank2edge, id, w, old, edge2perm_rank, icp);
  }
}

void EdgeHeap::removeEdge(const Edge e) {
  EdgeId id = e.id();
  EdgeWeight oldF = icf[id];
  EdgeWeight oldP = icp[id];
  icf[id] = LightCompleteGraph::Forbidden;
  icp[id] = LightCompleteGraph::Forbidden;
  updateHeap(forb_rank2edge, id, LightCompleteGraph::Forbidden, oldF, edge2forb_rank, icf);
  updateHeap(perm_rank2edge, id, LightCompleteGraph::Forbidden, oldP, edge2perm_rank, icp);
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
  EdgeWeight maxIcf = LightCompleteGraph::Forbidden;
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

void EdgeHeap::updateHeap(std::vector<EdgeId>& heap, const EdgeId e, const EdgeWeight newW, const EdgeWeight oldW, std::vector<EdgeId>& index, const std::vector<EdgeWeight>& score) {
  unsigned int pos = index[e];
  if (newW > oldW) {
    // value increased -> move edge upwards in heap
    while(score[heap[pos/2]] < score[heap[pos]]) {
      // swap pos and pos/2
      std::swap(heap[pos], heap[pos/2]);
      index[heap[pos]] = pos;
      index[heap[pos/2]] = pos/2;
      pos = pos/2;
    }
  } else {
    // value decreased -> move edge downwards in heap
    while((2*pos < heap.size() && score[heap[pos]] < score[heap[2*pos]])
	|| (2*pos+1 < heap.size() && score[heap[pos]] < score[heap[2*pos+1]]) ) {
      if (2*pos+1 < heap.size() && score[heap[2*pos]] < score[heap[2*pos+1]]) {
	// element 2*pos+1 exists and is larger than 2*pos -> swap pos with 2*pos+1
	std::swap(heap[pos], heap[2*pos+1]);
	index[heap[pos]] = pos;
	index[heap[pos*2+1]] = pos*2+1;
	pos = 2*pos+1;
      } else {
	// else swap with 2*pos
	std::swap(heap[pos], heap[2*pos]);
	index[heap[pos]] = pos;
	index[heap[pos*2]] = pos*2;
	pos = 2*pos;
      }
    }
  }
//   bool wrong = false;
//   unsigned int where = 0;
//   for (unsigned int i = 0; i < heap.size()/2; i++) {
//     if (score[heap[i]] < score[heap[2*i]]) {
//       wrong = true;
//       where = i;
//     }
//     if (2*i+1 < heap.size() && score[heap[i]] < score[heap[2*i+1]]) {
//       wrong = true;
//       where = i;
//     }
//   }
//   if (wrong) {
//     std::cout<<"#######R Error in heap ("<<where<<") "<<score[heap[where/2]]<<" >= "<<score[heap[where]]<<" >= "<<score[heap[2*where]]<<" & "<<score[heap[2*where+1]]<<std::endl;
//   }
}

} // namespace ysk