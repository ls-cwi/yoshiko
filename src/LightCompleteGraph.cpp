#include "LightCompleteGraph.h"

namespace ysk {

const LightCompleteGraph::EdgeWeight LightCompleteGraph::Forbidden = -std::numeric_limits< EdgeWeight >::infinity();
const LightCompleteGraph::EdgeWeight LightCompleteGraph::Permanent = std::numeric_limits< EdgeWeight >::infinity();
const LightCompleteGraph::Edge LightCompleteGraph::InvalidEdge = {std::numeric_limits<NodeId>::max(), std::numeric_limits<NodeId>::max()};
const LightCompleteGraph::EdgeId LightCompleteGraph::InvalidEdgeId = -1;
  
LightCompleteGraph::LightCompleteGraph(WorkingCopyInstance& inst) :
  size(inst.getGraph().nodeNum()),
  weights(size*(size-1)/2)
{
  for (int i = 0; i < inst.getGraph().nodeNum(); i++) {
    for (int j = i+1; j < inst.getGraph().nodeNum(); j++) {
      Edge e(j,i);
      WorkingCopyGraph::Edge we = inst.edge(inst.getGraph().nodeFromId(size-i-1), inst.getGraph().nodeFromId(size-j-1));
      weights[e.id()] = (EdgeWeight)inst.getWeight(we);
    }
  }
}

LightCompleteGraph::LightCompleteGraph(LightCompleteGraph& other) :
  size(other.size),
  weights(other.weights)
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

} // namespace ysk