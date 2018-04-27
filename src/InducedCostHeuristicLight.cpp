#include "InducedCostHeuristicLight.h"

namespace ysk {
  
using Edge = LightCompleteGraph::Edge;
using EdgeWeight = LightCompleteGraph::EdgeWeight;
using EdgeId = LightCompleteGraph::EdgeId;
using NodeId = LightCompleteGraph::NodeId;

InducedCostHeuristicLight::InducedCostHeuristicLight(LightCompleteGraph& param_graph) :
  graph(param_graph),
  edgeHeap(graph),
  totalCost(0.0)
{
}

ClusterEditingSolutionLight InducedCostHeuristicLight::solve() {
  // execute algorithm
  if (verbosity >= 1)
    std::cout<<"Running heuristic." << "."<<std::endl;
  for (unsigned int i = 0; i < graph.numEdges() + 1; i++) {
    Edge eIcf = edgeHeap.getMaxIcfEdge();
    Edge eIcp = edgeHeap.getMaxIcpEdge();
    if (eIcf == LightCompleteGraph::InvalidEdge || eIcp == LightCompleteGraph::InvalidEdge) {
      break;
    }
    EdgeWeight mIcf = edgeHeap.getIcf(eIcf);
    EdgeWeight mIcp = edgeHeap.getIcp(eIcp);
//     std::cout<<"mIcf = ("<< mIcf <<")  mIcp = ("<< mIcp <<")"<<std::endl;
    if (mIcf >= mIcp) {
      // set eIcf to permanent
      setPermanent(eIcf);
      edgeHeap.removeEdge(eIcf);
    } else {
      // set eIcp fo forbidden
      setForbidden(eIcp);
      edgeHeap.removeEdge(eIcp);
    }
    if (verbosity >= 2 && i%250 == 0) {
      std::cout<<"Completed "<<(i*100/graph.numEdges())<<"%\r"<<std::flush;
    }
  }
  
  // calculate clustering
  if (verbosity >= 1)
    std::cout<<"Constructing result."<<std::endl;
  std::vector<std::vector<NodeId>> clusters;
  std::vector<int> clusterOfNode(graph.numNodes(), -1);
  for (NodeId u = 0; u < graph.numNodes(); u++) {
    // add cluster if not explored yet
    std::cout<<"Completed "<<(u*100/graph.numNodes())<<"%\r"<<std::flush;
    if (clusterOfNode[u] == -1) {
      int c = clusters.size();
      clusterOfNode[u] = c;
      clusters.push_back(std::vector<NodeId>(1, u));
      for (NodeId v = u+1; v < graph.numNodes(); v++) {
	if (graph.getWeight(Edge(u, v)) == LightCompleteGraph::Permanent) {
	  clusterOfNode[v] = c;
	  clusters[c].push_back(v);
	}
      }
    }
  }
  return ClusterEditingSolutionLight(totalCost, clusters);
}

void InducedCostHeuristicLight::setForbidden(const Edge e) {
  if (graph.getWeight(e) == LightCompleteGraph::Forbidden) {
    return;
  }
  NodeId u = e.u;
  NodeId v = e.v;
  for (NodeId w = 0; w < graph.numNodes(); w++) {
    if (w == u || w == v)
      continue;
    Edge uw(u, w);
    Edge vw(v, w);
    updateTripleForbiddenUW(e, uw, vw);
    updateTripleForbiddenUW(e, vw, uw);
  }
  if (graph.getWeight(e) > 0)
    totalCost += graph.getWeight(e);
  graph.setWeight(e, LightCompleteGraph::Forbidden);
}

void InducedCostHeuristicLight::setPermanent(const Edge e) {
  if (graph.getWeight(e) == LightCompleteGraph::Permanent) {
    return;
  }
  NodeId u = e.u;
  NodeId v = e.v;
  for (NodeId w = 0; w < graph.numNodes(); w++) {
    if (w == u || w == v)
      continue;
    Edge uw(u, w);
    Edge vw(v, w);
    updateTriplePermanentUW(e, uw, vw);
    updateTriplePermanentUW(e, vw, uw);
  }
  if (graph.getWeight(e) < 0)
    totalCost -= graph.getWeight(e);
  graph.setWeight(e, LightCompleteGraph::Permanent);
}

void InducedCostHeuristicLight::updateTripleForbiddenUW(const Edge uv, const Edge uw, const Edge vw) {
  EdgeWeight icf_old = edgeHeap.getIcf(graph.getWeight(uv), graph.getWeight(vw));
  EdgeWeight icf_new = edgeHeap.getIcf(LightCompleteGraph::Forbidden, graph.getWeight(vw));
  EdgeWeight icp_old = edgeHeap.getIcp(graph.getWeight(uv), graph.getWeight(vw));
  EdgeWeight icp_new = edgeHeap.getIcp(LightCompleteGraph::Forbidden, graph.getWeight(vw));
  edgeHeap.increaseIcf(uw, icf_new - icf_old);
  edgeHeap.increaseIcp(uw, icp_new - icp_old);
}

void InducedCostHeuristicLight::updateTriplePermanentUW(const Edge uv, const Edge uw, const Edge vw) {
  EdgeWeight icf_old = edgeHeap.getIcf(graph.getWeight(uv), graph.getWeight(vw));
  EdgeWeight icf_new = edgeHeap.getIcf(LightCompleteGraph::Permanent, graph.getWeight(vw));
  EdgeWeight icp_old = edgeHeap.getIcp(graph.getWeight(uv), graph.getWeight(vw));
  EdgeWeight icp_new = edgeHeap.getIcp(LightCompleteGraph::Permanent, graph.getWeight(vw));
  edgeHeap.increaseIcf(uw, icf_new - icf_old);
  edgeHeap.increaseIcp(uw, icp_new - icp_old);
  edgeHeap.increaseIcf(uw, icf_new - icf_old);
  edgeHeap.increaseIcp(uw, icp_new - icp_old);
}

} // namespace ysk