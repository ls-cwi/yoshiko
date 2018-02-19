#include "MergingRule.h"

using namespace std;
using namespace lemon;

namespace ysk {
  
void MergingRule::apply() {
  WorkingCopyGraph::NodeMap<bool> nodes(_instance.getGraph(), true);
  SubGraph<const WorkingCopyGraph, WorkingCopyGraph::NodeMap<bool>, const WorkingCopyGraph::EdgeMap<bool> > g(_instance.getGraph(), nodes, _instance.getPermanent());
  SubGraph<const WorkingCopyGraph, WorkingCopyGraph::NodeMap<bool>, const WorkingCopyGraph::EdgeMap<bool>  >::NodeMap<int> cluster(g, -1);
  
  int count = connectedComponents(g, cluster);
  
  for(int i = 0; i < count; i++) {
    vector<WorkingCopyGraph::Node> m;
    for(WorkingCopyGraph::NodeIt u(_instance.getGraph()); u != INVALID; ++u) {
      if(cluster[u] == i)
        m.push_back(u);
    }
    if(m.size() >= 2) {
      _success = true;
      _instance.merge(m);
    }
  }
}
  
} // namespace ysk
