#include "ClusterEditingSolutionLight.h"
#include "ClusterEditingSolutions.h"

namespace ysk {
  
using NodeId = LightCompleteGraph::NodeId;

ClusterEditingSolutionLight::ClusterEditingSolutionLight(double pTotalCost, std::vector<std::vector<NodeId>> &pClusters) :
  totalCost(pTotalCost),
  clusters(pClusters)
{}

double ClusterEditingSolutionLight::getTotalCost() const {
  return totalCost;
}

unsigned int ClusterEditingSolutionLight::getNumClusters() const {
  return clusters.size();
}

const std::vector<NodeId>& ClusterEditingSolutionLight::getCluster(const unsigned int index) const {
  return clusters[index];
}

} //namespace ysk