//
//  ClusterEditingReduction.cpp
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 18.11.12.
//  Copyright (c) 2012 Emanuel Laude. All rights reserved.
//

#include "ClusterEditingReduction.h"

using namespace std;

namespace ysk {

ClusterEditingReduction::~ClusterEditingReduction() {
  for(vector<ClusterReductionInstance*>::iterator it = _instances.begin(); it != _instances.end(); it++) {
    delete *it;
  }
}

void ClusterEditingReduction::perform(ClusterEditingInstance& orig) {
  vector<ClusterReductionInstance*> instances;
  ClusterReductionInstance i(&orig, _active, _multiplicativeFactor, _conserveMultipleSolutions);
  i.computeChildInstances(instances);

  while(instances.size() != 0) {
    vector<ClusterReductionInstance*> tmp;

    for(vector<ClusterReductionInstance*>::iterator it = instances.begin(); it != instances.end(); it++) {
      int i = (*it)->applyReductionRules(_informer);

      if(i != -1) {
        double cost = (*it)->getInstance()->computeCost();
        _totalCost += cost;
        if(verbosity > 1)
          cout << "cost:\t"<<cost<<endl;

        vector<ClusterReductionInstance*> childInstances;
        (*it)->computeChildInstances(childInstances);
        tmp.insert(tmp.end(), childInstances.begin(), childInstances.end());
        delete (*it)->getInstance();
        delete *it;
      } else {
        _instances.push_back(*it);
      }
    }
    instances = tmp;
  }
}

//SETTER / GETTER

void ClusterEditingReduction::registerInformer(yskLib::CplexInformer* informer){
	_informer = informer;
}

vector<ClusterReductionInstance*>& ClusterEditingReduction::getInstances() {
  return _instances;
}

double ClusterEditingReduction::getTotalCost() const {
  return _totalCost;
}

} // namespace ysk
