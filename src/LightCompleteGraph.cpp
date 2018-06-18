#include "LightCompleteGraph.h"

namespace ysk {

const LightCompleteGraph::EdgeWeight LightCompleteGraph::Forbidden = -std::numeric_limits< EdgeWeight >::infinity();
const LightCompleteGraph::EdgeWeight LightCompleteGraph::Permanent = std::numeric_limits< EdgeWeight >::infinity();
const LightCompleteGraph::Edge LightCompleteGraph::InvalidEdge = {std::numeric_limits<NodeId>::max(), std::numeric_limits<NodeId>::max()};
const LightCompleteGraph::EdgeId LightCompleteGraph::InvalidEdgeId = -1;
const LightCompleteGraph::NodeId LightCompleteGraph::InvalidNodeId = -1;
  
LightCompleteGraph::LightCompleteGraph(WorkingCopyInstance& inst, bool param_pruneZeroEdges) :
  size(inst.getGraph().nodeNum()),
  weights(size*(size-1)/2),
  pruneZeroEdges(param_pruneZeroEdges),
//   origToCompr(inst.getGraph().nodeNum()),
//   comprToOrig(inst.getGraph().nodeNum(), vector<NodeId>(1, InvalidNodeId)),
  cliqueOf(inst.getGraph().nodeNum()),
  unprunedNeighbours(inst.getGraph().nodeNum(), vector<NodeId>(0))
{
  for (int i = 0; i < inst.getGraph().nodeNum(); i++) {
    cliqueOf[i].push_back(i);
    for (int j = i+1; j < inst.getGraph().nodeNum(); j++) {
      Edge e(j,i);
      WorkingCopyGraph::Edge we = inst.edge(inst.getGraph().nodeFromId(i), inst.getGraph().nodeFromId(j));
      weights[e.id()] = (EdgeWeight)inst.getWeight(we);
      if (weights[e.id()] == Permanent) {
	cliqueOf[i].push_back(j);
	cliqueOf[j].push_back(i);
      } else if (weights[e.id()] != Forbidden && weights[e.id()] != Permanent && (weights[e.id()] != 0.0 || !pruneZeroEdges)) {
	unprunedNeighbours[i].push_back(j);
	unprunedNeighbours[j].push_back(i);
      }
    }
//     origToCompr[i] = i;
//     comprToOrig[i][0] = i;
  }
}

LightCompleteGraph::LightCompleteGraph(LightCompleteGraph& other) :
  size(other.size),
  weights(other.weights),
  pruneZeroEdges(other.pruneZeroEdges),
//   origToCompr(other.origToCompr),
//   comprToOrig(other.comprToOrig),
  cliqueOf(other.cliqueOf),
  unprunedNeighbours(other.unprunedNeighbours)
{
}

double LightCompleteGraph::getWeight(const Edge e) const {
  return weights[e.id()];
}

void LightCompleteGraph::setWeight(const Edge e, const EdgeWeight w) {
  if (weights[e.id()] != Permanent && w == Permanent) {
    cliqueOf[e.u].push_back(e.v);
    cliqueOf[e.v].push_back(e.u);
  } else if (weights[e.id()] == Permanent && w != Permanent) {
    if (!removeFromVector(cliqueOf[e.u], e.v))
      std::cout<<"Error: Permanent neighbour not found"<<std::endl;
    if (!removeFromVector(cliqueOf[e.v], e.u))
      std::cout<<"Error: Permanent neighbour not found"<<std::endl;
  }
  if ((weights[e.id()] == Forbidden || weights[e.id()] == Permanent || (weights[e.id()] == 0.0 && pruneZeroEdges)) && ((w != 0.0 || !pruneZeroEdges) && w != Forbidden && w != Permanent)) {
    unprunedNeighbours[e.u].push_back(e.v);
    unprunedNeighbours[e.v].push_back(e.u);
  } else if (weights[e.id()] != Forbidden && weights[e.id()] != Permanent && (weights[e.id()] != 0.0 || !pruneZeroEdges) && ((w == 0.0 && pruneZeroEdges) || w == Forbidden || w == Permanent)) {
    if (!removeFromVector(unprunedNeighbours[e.u], e.v))
      std::cout<<"Error: Non-zero real neighbour not found"<<std::endl;
    if (!removeFromVector(unprunedNeighbours[e.v], e.u))
      std::cout<<"Error: Non-zero real neighbour not found"<<std::endl;
  }
  weights[e.id()] = w;
}

unsigned int LightCompleteGraph::numNodes() const {
  return size;
}

unsigned long LightCompleteGraph::numEdges() const {
  return weights.size();
}

const std::vector<LightCompleteGraph::NodeId>& LightCompleteGraph::getCliqueOf(const LightCompleteGraph::NodeId v) const {
  return cliqueOf[v];
}

const std::vector<LightCompleteGraph::NodeId>& LightCompleteGraph::getUnprunedNeighbours(const LightCompleteGraph::NodeId v) const {
  return unprunedNeighbours[v];
}

// void LightCompleteGraph::contract(const LightCompleteGraph::Edge e) {
//   NodeId u = e.u;
//   NodeId v = e.v;
//   NodeId last = numNodes() - 1;
//   
//   for (NodeId w = 0; w < numNodes(); w++) {
//     if (w == u || w == v)
//       continue;
//     Edge uw(u, w);
//     Edge vw(v, w);
//     Edge wl(w, last);
//     EdgeWeight weight_V = getWeight(vw);
//     // combine edge weights of u and v
//     setWeight(uw, getWeight(uw) + weight_V);
//     // swap v with last node, by assigning edge weights of last node to v
//     setWeight(wl, weight_V);
//   }
//   
//   // link changes
//   origToCompr[last] = v;
//   origToCompr[v] = u;
//   comprToOrig[u].push_back(v);
//   
//   // delete last internal node
//   size--;
//   weights.resize(size*(size-1)/2);
// }
/*
LightCompleteGraph::NodeId LightCompleteGraph::getInternalId(const LightCompleteGraph::NodeId v) const {
  return origToCompr[v];
}

LightCompleteGraph::vector< uint32_t > LightCompleteGraph::getOriginalIds(const LightCompleteGraph::NodeId v) const {
  return comprToOrig[v];
}*/

bool LightCompleteGraph::removeFromVector(std::vector<NodeId> vec, LightCompleteGraph::NodeId v) {
  for (unsigned int i = 0; i < vec.size(); i++) {
    if (vec[i] == v) {
      vec.erase(vec.begin()+i);
      return true;
    }
  }
  return false;
}


} // namespace ysk