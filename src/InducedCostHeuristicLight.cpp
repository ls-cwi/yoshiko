#include "InducedCostHeuristicLight.h"

namespace ysk {
  
using Edge = LightCompleteGraph::Edge;
using EdgeWeight = LightCompleteGraph::EdgeWeight;
using EdgeId = LightCompleteGraph::EdgeId;
using NodeId = LightCompleteGraph::NodeId;

InducedCostHeuristicLight::InducedCostHeuristicLight(LightCompleteGraph& param_graph) :
  graph(param_graph),
  edges(graph),
  totalCost(0.0)
{
}

ClusterEditingSolutionLight InducedCostHeuristicLight::solve() {
  // execute algorithm
  std::cout<<"Running heuristic."<<std::endl;
  for (unsigned int i = 0; i < graph.numEdges() + 1; i++) {
    if (i%10000 == 0) {
      std::cout<<"Completed "<<(i*100/graph.numEdges())<<"%."<<std::endl;
    }
    Edge eIcf = edges.getMaxIcfEdge();
    Edge eIcp = edges.getMaxIcpEdge();
    if (eIcf == LightCompleteGraph::InvalidEdge || eIcp == LightCompleteGraph::InvalidEdge)
      break;
    EdgeWeight mIcf = edges.getIcf(eIcf);
    EdgeWeight mIcp = edges.getIcp(eIcp);
    if (mIcf >= mIcp) {
      // set eIcf to permanent
      setPermanent(eIcf);
      edges.removeEdge(eIcf);
    } else {
      // set eIcp fo forbidden
      setForbidden(eIcp);
      edges.removeEdge(eIcp);
    }
  }
  
  // calculate clustering
  std::cout<<"Constructing result."<<std::endl;
  std::vector<std::vector<NodeId>> clusters;
  std::vector<int> clusterOfNode(graph.numNodes(), -1);
  for (NodeId u = 0; u < graph.numNodes(); u++) {
    // add cluster if not explored yet
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
  NodeId u = e.u;
  NodeId v = e.v;
  for (NodeId w = 0; w < graph.numNodes(); w++) {
    if (w == u || w == v)
      continue;
    updateTripleForbiddenUW(u, v, w);
    updateTripleForbiddenUW(v, u, w);
  }
  if (graph.getWeight(e) > 0)
    totalCost += graph.getWeight(e);
  graph.setWeight(e, LightCompleteGraph::Forbidden);
}

void InducedCostHeuristicLight::setPermanent(const Edge e) {
  NodeId u = e.u;
  NodeId v = e.v;
  for (NodeId w = 0; w < graph.numNodes(); w++) {
    if (w == u || w == v)
      continue;
    updateTriplePermanentUW(u, v, w);
    updateTriplePermanentUW(v, u, w);
  }
  if (graph.getWeight(e) < 0)
    totalCost -= graph.getWeight(e);
  graph.setWeight(e, LightCompleteGraph::Permanent);
}

void InducedCostHeuristicLight::updateTripleForbiddenUW(const NodeId u, const NodeId v, const NodeId w) {
  EdgeWeight icf_old = edges.getIcf(graph.getWeight(Edge(u,w)), graph.getWeight(Edge(u,v)), graph.getWeight(Edge(v,w)));
  EdgeWeight icf_new = edges.getIcf(graph.getWeight(Edge(u,w)), LightCompleteGraph::Forbidden, graph.getWeight(Edge(v,w)));
  EdgeWeight icp_old = edges.getIcp(graph.getWeight(Edge(u,w)), graph.getWeight(Edge(u,v)), graph.getWeight(Edge(v,w)));
  EdgeWeight icp_new = edges.getIcp(graph.getWeight(Edge(u,w)), LightCompleteGraph::Forbidden, graph.getWeight(Edge(v,w)));
  edges.updateIcf(Edge(u,w), edges.getIcf(Edge(u,w)) + icf_new - icf_old);
  edges.updateIcp(Edge(u,w), edges.getIcp(Edge(u,w)) + icp_new - icp_old);
}

void InducedCostHeuristicLight::updateTriplePermanentUW(const NodeId u, const NodeId v, const NodeId w) {
  EdgeWeight icf_old = edges.getIcf(graph.getWeight(Edge(u,w)), graph.getWeight(Edge(u,v)), graph.getWeight(Edge(v,w)));
  EdgeWeight icf_new = edges.getIcf(graph.getWeight(Edge(u,w)), LightCompleteGraph::Permanent, graph.getWeight(Edge(v,w)));
  EdgeWeight icp_old = edges.getIcp(graph.getWeight(Edge(u,w)), graph.getWeight(Edge(u,v)), graph.getWeight(Edge(v,w)));
  EdgeWeight icp_new = edges.getIcp(graph.getWeight(Edge(u,w)), LightCompleteGraph::Permanent, graph.getWeight(Edge(v,w)));
  edges.updateIcf(Edge(u,w), edges.getIcf(Edge(u,w)) + icf_new - icf_old);
  edges.updateIcp(Edge(u,w), edges.getIcp(Edge(u,w)) + icp_new - icp_old);
}

} // namespace ysk