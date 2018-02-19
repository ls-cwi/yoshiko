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
	, _informer(nullptr)
    , _multiplicativeFactor(multiplicativeFactor)
    , _conserveMultipleSolutions(conserveMultipleSolutions)
  {
  }

  void perform(ClusterEditingInstance& orig);

  void registerInformer(yskLib::CplexInformer* informer);

  virtual ~ClusterEditingReduction();

  std::vector<ClusterReductionInstance*>& getInstances();

  double getTotalCost() const;

private:
  std::vector<ClusterReductionInstance*> _instances;
  double _totalCost;
  std::bitset<NUMBER_OF_REDUCTION_RULES> _active;
  yskLib::CplexInformer* _informer;
  /**
   *
   */
  double _multiplicativeFactor;
  bool _conserveMultipleSolutions;
};

} // namespace ysk

#endif /* CLUSTEREDITINGREDUCTION_H */
