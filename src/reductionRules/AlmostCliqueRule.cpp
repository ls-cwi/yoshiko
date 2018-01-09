/*
 * File:   AlmostCliqueRule.cpp
 * Author: emanuellaude
 *
 * Created on 26. August 2012, 11:35
 */

#include "AlmostCliqueRule.h"

using namespace lemon;
using namespace std;

namespace ysk {

void AlmostCliqueRule::apply() {
  init();
  
  bool terminate = false;
  
  while (!terminate && _connectivity.size() > 0) {
    WorkingCopyGraph::NodeMap<bool> candidate(_instance.getGraph(), false);
    WorkingCopyGraph::NodeMap<double> connectivityCandidate(_instance.getGraph(), 0.0);
    
    
    
    WorkingCopyGraph::Node start = _connectivity.top();
    _connectivity.pop();
    
    
    candidate[start] = true;
    
    double prevConnectivity = 0;
    
    
    WorkingCopyGraph::Node u = findNodeMaxConnectivity(start, candidate, connectivityCandidate);
    if(u == INVALID) {
      terminate = true;
      
    }
    while (u != INVALID && connectivityCandidate[u] >= prevConnectivity && !terminate) {
      candidate[u] = true;
      
      prevConnectivity = connectivityCandidate[u];
      
      //if u is connected to more nodes in V\C than to nodes in C terminate!
      int internal = 0;
      int external = 0;
      
      for(WorkingCopyGraph::IncEdgeIt uv(_instance.getGraph(), u); uv != INVALID; ++uv) {
        if(_instance.getWeight(uv) <= 0) {
          continue;
        }
        
        WorkingCopyGraph::Node v = _instance.getGraph().oppositeNode(u, uv);
        if(candidate[v]) {
          internal++;
        } else {
          external++;
        }
      }
      
      int bound = (int) ceil((double) countNodes(_instance.getGraph()) / 2)* internal;
      if(bound <= external) {
        terminate = true;
      } else {
        u = findNodeMaxConnectivity(u, candidate, connectivityCandidate);
        if(u == INVALID) {
          terminate = true;
        }
      }
    }
    
    if(mapCount(_instance.getGraph(), candidate, true) < 2) {
      continue;
    }
    
    double cost = computeCost(candidate);
    double minCutValue = computeMinCutValue(candidate);
    
    if ((cost <= minCutValue && !_conserveMultipleSolutions) || (cost < minCutValue && _conserveMultipleSolutions)) {
      _success = true;
      merge(candidate);
    }
  }
}

double AlmostCliqueRule::computeMinCutValue(WorkingCopyGraph::NodeMap<bool>& candidate) {
  ConstMap<WorkingCopyGraph::Edge, double> zero(0.0);
  typedef LessMap<ConstMap<WorkingCopyGraph::Edge, double>, const WorkingCopyGraph::EdgeMap<double> > PositiveWeight;
  PositiveWeight posWeight(zero, _instance.getWeight());
  
  typedef SubGraph<const WorkingCopyGraph, WorkingCopyGraph::NodeMap<bool>, PositiveWeight> SubGraphPositiveWeight;
  SubGraphPositiveWeight candidatePosWeight(_instance.getGraph(), candidate, posWeight);
  
  HaoOrlin<SubGraphPositiveWeight, WorkingCopyGraph::EdgeMap<double> > minCut(candidatePosWeight, _instance.getWeight());
  minCut.init();
  minCut.calculateOut();
  return minCut.minCutValue();
}

double AlmostCliqueRule::computeCost(WorkingCopyGraph::NodeMap<bool>& candidate) {
  double cost = 0.0;
  
  for (WorkingCopyGraph::NodeIt u(_instance.getGraph()); u != INVALID; ++u) {
    if(!candidate[u]) {
      continue;
    }
    
    WorkingCopyGraph::NodeIt v(_instance.getGraph(), u);
    ++v;
    for (; v != INVALID; ++v) {
      if(!candidate[v]) {
        continue;
      }
      
      double weight = _instance.getWeight(_instance.edge(u, v));
      if (weight < 0)
        cost += -1 * weight;
    }
    
    
    for (WorkingCopyGraph::NodeIt v(_instance.getGraph()); v != INVALID; ++v) {
      if(candidate[v]) {
        continue;
      }
      double weight = _instance.getWeight(_instance.edge(u, v));
      if (weight > 0)
        cost += weight;
    }
  }
  return cost;
}

WorkingCopyGraph::Node AlmostCliqueRule::findNodeMaxConnectivity(WorkingCopyGraph::Node& lastAdded, WorkingCopyGraph::NodeMap<bool>& candidate, WorkingCopyGraph::NodeMap<double>& connectivityCandidate) {
  WorkingCopyGraph::Node nodeMaxConnectivity = INVALID;
  double maxConnectivity = -std::numeric_limits<double>::infinity();
  for (WorkingCopyGraph::NodeIt u(_instance.getGraph()); u != INVALID; ++u) {
    if(candidate[u]) {
      continue; //u already is in candidate set.
    }
    
    connectivityCandidate[u] = connectivityCandidate[u] + _instance.getWeight(_instance.edge(u, lastAdded));
    if (connectivityCandidate[u] > maxConnectivity) {
      maxConnectivity = connectivityCandidate[u];
      nodeMaxConnectivity = u;
    }
  }
  return nodeMaxConnectivity;
}

void AlmostCliqueRule::merge(WorkingCopyGraph::NodeMap<bool> &candidate) {
  vector<WorkingCopyGraph::Node> nodes;
  for(WorkingCopyGraph::NodeIt u(_instance.getGraph()); u != INVALID; ++u) {
    if(candidate[u]) {
      if(_connectivity.state(u) == MaxHeap::IN_HEAP) {
        _connectivity.erase(u);
      }
      
      nodes.push_back(u);
    }
  }
  
  WorkingCopyGraph::Node proxy = _instance.merge(nodes);
  
  //compute new connectivity of proxy to add proxy node to connectivity maxheap again.
  double connectivity = 0.0;
  
  for(WorkingCopyGraph::IncEdgeIt e(_instance.getGraph(), proxy); e != INVALID; ++e) {
    connectivity += _instance.getWeight(e);
  }
  
  if(_connectivity.state(proxy) == MaxHeap::IN_HEAP) {
    exit(-1);
  }
  
  _connectivity.push(proxy, connectivity);
}

void AlmostCliqueRule::init() {
  for (WorkingCopyGraph::NodeIt u(_instance.getGraph()); u != INVALID; ++u) {
    double connectivity = 0;
    
    for (WorkingCopyGraph::IncEdgeIt uv(_instance.getGraph(), u); uv != INVALID; ++uv) {
      connectivity += _instance.getWeight(uv);
    }
    
    _connectivity.push(u, connectivity);
  }
}

} // namespace ysk
