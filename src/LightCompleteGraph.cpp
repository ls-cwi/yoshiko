#include "LightCompleteGraph.h"

namespace ysk {

const LightCompleteGraph::EdgeWeight LightCompleteGraph::Forbidden = -std::numeric_limits< EdgeWeight >::infinity();
const LightCompleteGraph::EdgeWeight LightCompleteGraph::Permanent = std::numeric_limits< EdgeWeight >::infinity();
const LightCompleteGraph::Edge LightCompleteGraph::InvalidEdge = {std::numeric_limits<NodeId>::max(), std::numeric_limits<NodeId>::max()};
const LightCompleteGraph::EdgeId LightCompleteGraph::InvalidEdgeId = -1;
const LightCompleteGraph::EdgeId LightCompleteGraph::InvalidNodeId = -1;
  
LightCompleteGraph::LightCompleteGraph(WorkingCopyInstance& inst) :
  size(inst.getGraph().nodeNum()),
  weights(size*(size-1)/2),
  origToCompr(inst.getGraph().nodeNum()),
  comprToOrig(inst.getGraph().nodeNum(), vector<NodeId>(1, InvalidNodeId))
{
  for (int i = 0; i < inst.getGraph().nodeNum(); i++) {
    for (int j = i+1; j < inst.getGraph().nodeNum(); j++) {
      Edge e(j,i);
      WorkingCopyGraph::Edge we = inst.edge(inst.getGraph().nodeFromId(i), inst.getGraph().nodeFromId(j));
      weights[e.id()] = (EdgeWeight)inst.getWeight(we);
    }
    origToCompr[i] = i;
    comprToOrig[i][0] = i;
  }
}

LightCompleteGraph::LightCompleteGraph(LightCompleteGraph& other) :
  size(other.size),
  weights(other.weights),
  origToCompr(other.origToCompr),
  comprToOrig(other.comprToOrig)
{
}

double LightCompleteGraph::getWeight(const Edge e) const {
  return weights[e.id()];
}

void LightCompleteGraph::setWeight(const Edge e, const EdgeWeight w) {
  weights[e.id()] = w;
}

unsigned int LightCompleteGraph::numNodes() const {
  return size;
}

unsigned long LightCompleteGraph::numEdges() const {
  return weights.size();
}

void LightCompleteGraph::contract(const LightCompleteGraph::Edge e) {
  NodeId u = e.u;
  NodeId v = e.v;
  NodeId last = numNodes() - 1;
  
  for (NodeId w = 0; w < numNodes(); w++) {
    if (w == u || w == v)
      continue;
    Edge uw(u, w);
    Edge vw(v, w);
    Edge wl(w, last);
    EdgeWeight weight_V = getWeight(vw);
    // combine edge weights of u and v
    setWeight(uw, getWeight(uw) + weight_V;
    // swap v with last node, by assigning edge weights of last node to v
    setWeight(wl, weight_V);
  }
  
  // link changes
  origToCompr[last] = v;
  origToCompr[v] = u;
  comprToOrig[u].push_back(v);
  
  // delete last internal node
  size--;
  weights.resize(size*(size-1)/2);
}

LightCompleteGraph::NodeId LightCompleteGraph::getInternalId(const LightCompleteGraph::NodeId v) const {
  return origToCompr[v];
}

LightCompleteGraph::vector< uint32_t > LightCompleteGraph::getOriginalIds(const LightCompleteGraph::NodeId v) const {
  return comprToOrig[v];
}

} // namespace ysk