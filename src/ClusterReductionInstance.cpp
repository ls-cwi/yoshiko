//
//  ClusterReductionInstance.cpp
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 12.01.13.
//  Copyright (c) 2013 Emanuel Laude. All rights reserved.
//

#include "ClusterReductionInstance.h"

using namespace std;
using namespace lemon;

namespace ysk {

int ClusterReductionInstance::applyReductionRules() {
  if(_currentRule < 0 || _currentRule > NUMBER_OF_REDUCTION_RULES-1) {
    return -1;
  }

  if(countNodes(_instance->getWorkingCopyInstance().getGraph()) <= 1) {
    return -1;
  }

  if(!_active.any()) {
    return -1;
  }

  //Iterate over rules until one rule successfully reduced instance. =>
  for(; (_currentRule <= NUMBER_OF_REDUCTION_RULES-1 && _lastSuccessRule == -1) || (_currentRule != _lastSuccessRule && _lastSuccessRule != -1); _currentRule = (_lastSuccessRule == -1 ? _currentRule+1 : (_currentRule+1) % NUMBER_OF_REDUCTION_RULES)) {

	if (isTerminated) return -1;

	if(_instance->getWorkingCopyInstance().isDirty()) {
      cerr << "Fatal error: Instance is dirty."<<endl;
      exit(-1);
    }
    //skip rule if rule is turned off and rule is not merging rule
    if(!_active.test(_currentRule) && (_currentRule != 1 || (_instance->isDualWeighted())))
      continue;

    FPTReductionRule* rule;
    if(_currentRule == 0) {
      rule = new CliqueRule(_instance->getWorkingCopyInstance());
    } else if(_currentRule == 1) {
      if(_instance->isDualWeighted()) {
        rule = new CriticalCliqueRule(_instance->getWorkingCopyInstance());
      } else {
        rule = new MergingRule(_instance->getWorkingCopyInstance());
      }
    } else if(_currentRule == 2) {
      rule = new AlmostCliqueRule(_instance->getWorkingCopyInstance(), _conserveMultipleSolutions);
    } else if(_currentRule == 3) {
      rule = new HeavyEdgeRule3in1(_instance->getWorkingCopyInstance(), _conserveMultipleSolutions);
    } else if(_currentRule == 4) {
      rule = new ParameterDependentReductionRule(_instance->getWorkingCopyInstance());
    } else if(_currentRule == 5) {
      rule = new SimilarNeighborhoodRule(_instance->getWorkingCopyInstance(), _multiplicativeFactor, _conserveMultipleSolutions);
    } else {
      return -1;
    }

    if(verbosity > 1) {
      cout <<"========================="<<endl;
      cout <<"number of nodes:\t"<<countNodes(_instance->getWorkingCopyInstance().getGraph())<<endl;
      cout<<"applying '"<<rule->getName()<<"'..."<<flush;
    }

    rule->apply();
    if(verbosity > 1)
      cout <<endl<<"done."<<endl;

    bool success = rule->isSuccess();

    if(verbosity > 1) {
      if(success) {
        cout<< "successfully reduced instance."<<endl;
        cout <<"number of nodes:\t"<<countNodes(_instance->getWorkingCopyInstance().getGraph())<<endl;
      } else {
        cout <<"failed to reduce instance."<<endl;
      }
    }

    delete rule;
    if(success) {
      _lastSuccessRule = _currentRule;
      return _currentRule;
    }
  }

  return -1;
}

ClusterEditingInstance* ClusterReductionInstance::getInstance() {
  return _instance;
}

int ClusterReductionInstance::computeConnectedComponents(WorkingCopyGraph::NodeMap<int>& connectedComps) {
  ConstMap<WorkingCopyGraph::Edge, double> zero(0.0);
  typedef LessMap<ConstMap<WorkingCopyGraph::Edge, double>, const WorkingCopyGraph::EdgeMap<double> > PositiveWeight;
  PositiveWeight posWeight(zero, _instance->getWorkingCopyInstance().getWeight());

  typedef SubGraph<const WorkingCopyGraph, WorkingCopyGraph::NodeMap<bool>, PositiveWeight> SubGraphPositiveWeight;
  WorkingCopyGraph::NodeMap<bool> nodes(_instance->getWorkingCopyInstance().getGraph(), true);
  SubGraphPositiveWeight subGraphPosWeight(_instance->getWorkingCopyInstance().getGraph(), nodes, posWeight);
  return connectedComponents(subGraphPosWeight, connectedComps);
}

void ClusterReductionInstance::computeChildInstances(vector<ClusterReductionInstance*>& childInstances) {
  WorkingCopyGraph::NodeMap<int> connectedComponents(_instance->getWorkingCopyInstance().getGraph(), -1);
  int count = computeConnectedComponents(connectedComponents);
  childInstances.resize(count);
  vector<int> size(count, 0);

  for(WorkingCopyGraph::NodeIt u(_instance->getWorkingCopyInstance().getGraph()); u != INVALID; ++u) {
    size[connectedComponents[u]]++;
  }

  if(verbosity > 1)
    cout << "number of connected components:    "<<count<<endl;

  for(int i = 0; i<count; i++) {
    childInstances[i] = new ClusterReductionInstance(new ClusterEditingInstance(), _active, _multiplicativeFactor, _conserveMultipleSolutions);
    if(count > 1) {
      childInstances[i]->_currentRule = 0;
      childInstances[i]->_lastSuccessRule = -1;
    } else {
      childInstances[i]->_currentRule = (_currentRule + 1) % NUMBER_OF_REDUCTION_RULES;
      childInstances[i]->_lastSuccessRule = _currentRule;
    }

    childInstances[i]->_instance->init(size[i]);

    WorkingCopyGraph::NodeIt u(_instance->getWorkingCopyInstance().getGraph());
    FullGraph::NodeIt x(childInstances[i]->_instance->getOrig());

    for(; u != INVALID; ++u) {
      if(connectedComponents[u] != i)
        continue;

      childInstances[i]->_instance->initNode(x, _instance->getWorkingCopyInstance().getNodeName(u), *_instance->getCluster(u));

      WorkingCopyGraph::NodeIt v(_instance->getWorkingCopyInstance().getGraph(), u);
      ++v;

      FullGraph::NodeIt y(childInstances[i]->_instance->getOrig(), x);
      ++y;
      for(; v != INVALID; ++v) {
        if(connectedComponents[v] != i)
          continue;

        childInstances[i]->_instance->initNode(y, _instance->getWorkingCopyInstance().getNodeName(v), *_instance->getCluster(v));

        WorkingCopyGraph::Edge uv = _instance->getWorkingCopyInstance().edge(u, v);
        FullGraph::Edge xy = childInstances[i]->_instance->getOrig().edge(x, y);
        childInstances[i]->_instance->initEdge(xy, _instance->getWorkingCopyInstance().getWeight(uv), _instance->getWorkingCopyInstance().getEdgeType(uv));
        childInstances[i]->_instance->getWorkingCopyInstance().edge(x, y);
        ++y;
      }

      ++x;
    }
  }
}

} // namespace ysk
