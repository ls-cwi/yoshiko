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
//   std::cout<<"1.1"<<std::endl;
}

ClusterEditingSolutionLight InducedCostHeuristicLight::solve() {
  
  for (unsigned int i = 0; i < graph.numEdges() + 1; i++) {
    Edge eIcf = edges.getMaxIcfEdge();
    Edge eIcp = edges.getMaxIcpEdge();
    if (eIcf == LightCompleteGraph::InvalidEdge || eIcp == LightCompleteGraph::InvalidEdge)
      break;
    EdgeWeight mIcf = edges.getIcf(eIcf);
    EdgeWeight mIcp = edges.getIcp(eIcp);
//     std::cout<<"Icf of ("<<eIcf.u<<","<<eIcf.v<<") = "<<mIcf<<" , Icp of ("<<eIcp.u<<","<<eIcp.v<<") = "<<mIcp<<"."<<std::endl;
    if (mIcf > mIcp) {
      // set eIcf to permanent
//       std::cout<<"Setting edge ("<<eIcf.u<<","<<eIcf.v<<") to permanent at weight "<<graph.getWeight(eIcf)<<std::endl;
      setPermanent(eIcf);
      edges.removeEdge(eIcf);
    } else {
//       std::cout<<"Setting edge ("<<eIcp.u<<","<<eIcp.v<<") to forbidden at weight "<<graph.getWeight(eIcp)<<std::endl;
      // set eIcp fo forbidden
      setForbidden(eIcp);
      edges.removeEdge(eIcp);
    }
//     std::cout<<"Total cost = "<<totalCost<<"."<<std::endl;
  }
  
//   std::cout<<"3"<<std::endl;
  
  // calculate clustering
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
  
  ClusterEditingSolutionLight sol(totalCost, clusters);
  return sol;
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
//   if (icf_old != icf_new)
//     std::cout<<"Icf_"<<v<<"("<<u<<","<<w<<") "<<icf_old<<" -> "<<icf_new<<std::endl;
//   if (icp_old != icp_new)
//     std::cout<<"Icp_"<<v<<"("<<u<<","<<w<<") "<<icp_old<<" -> "<<icp_new<<std::endl;
  edges.updateIcf(Edge(u,w), edges.getIcf(Edge(u,w)) + icf_new - icf_old);
  edges.updateIcp(Edge(u,w), edges.getIcp(Edge(u,w)) + icp_new - icp_old);
}

void InducedCostHeuristicLight::updateTriplePermanentUW(const NodeId u, const NodeId v, const NodeId w) {
  EdgeWeight icf_old = edges.getIcf(graph.getWeight(Edge(u,w)), graph.getWeight(Edge(u,v)), graph.getWeight(Edge(v,w)));
  EdgeWeight icf_new = edges.getIcf(graph.getWeight(Edge(u,w)), LightCompleteGraph::Permanent, graph.getWeight(Edge(v,w)));
  EdgeWeight icp_old = edges.getIcp(graph.getWeight(Edge(u,w)), graph.getWeight(Edge(u,v)), graph.getWeight(Edge(v,w)));
  EdgeWeight icp_new = edges.getIcp(graph.getWeight(Edge(u,w)), LightCompleteGraph::Permanent, graph.getWeight(Edge(v,w)));
//   if (icf_old != icf_new)
//     std::cout<<"Icf_"<<v<<"("<<u<<","<<w<<") "<<icf_old<<" -> "<<icf_new<<std::endl;
//   if (icp_old != icp_new)
//     std::cout<<"Icp_"<<v<<"("<<u<<","<<w<<") "<<icp_old<<" -> "<<icp_new<<std::endl;
  edges.updateIcf(Edge(u,w), edges.getIcf(Edge(u,w)) + icf_new - icf_old);
  edges.updateIcp(Edge(u,w), edges.getIcp(Edge(u,w)) + icp_new - icp_old);
}

} // namespace ysk