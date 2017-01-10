//
//  ClusterEditingReduction.h
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 18.11.12.
//  Copyright (c) 2012 Emanuel Laude. All rights reserved.
//

#ifndef CLUSTEREDITINGREDUCTION_H
#define CLUSTEREDITINGREDUCTION_H

#include <iostream>
#include <vector>
#include <string>
#include <bitset>

#include <lemon/connectivity.h>

#include "ClusterEditingInstance.h"
#include "ClusterReductionInstance.h"

namespace ysk {

class ClusterEditingReduction {
public:
  ClusterEditingReduction(std::bitset<NUMBER_OF_REDUCTION_RULES> active,
                          double multiplicativeFactor,
                          bool conserveMultipleSolutions)
    : _totalCost(0.0)
    , _active(active)
    , _multiplicativeFactor(multiplicativeFactor)
    , _conserveMultipleSolutions(conserveMultipleSolutions)
  {
  }
  
  void perform(ClusterEditingInstance& orig);
  
  virtual ~ClusterEditingReduction();
  
  std::vector<ClusterReductionInstance*>& getInstances();
  
  double getTotalCost() const;
  
private:
  std::vector<ClusterReductionInstance*> _instances;
  double _totalCost;
  std::bitset<NUMBER_OF_REDUCTION_RULES> _active;
  double _multiplicativeFactor;
  bool _conserveMultipleSolutions;
};
  
} // namespace ysk

#endif /* CLUSTEREDITINGREDUCTION_H */
