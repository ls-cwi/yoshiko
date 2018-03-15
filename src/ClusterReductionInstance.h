#ifndef CLUSTERREDUCTIONINSTANCE_H
#define CLUSTERREDUCTIONINSTANCE_H

#define NUMBER_OF_REDUCTION_RULES 6

#include "ClusterEditingInstance.h"
#include "CplexInformer.h"
#include "Globals.h"
#include "WorkingCopyInstance.h"

#include "reductionRules/HeavyEdgeRule3in1.h"
#include "reductionRules/CriticalCliqueRule.h"
#include "reductionRules/AlmostCliqueRule.h"
#include "reductionRules/SimilarNeighborhoodRule.h"
#include "reductionRules/MergingRule.h"
#include "reductionRules/ParameterDependentReductionRule.h"
#include "reductionRules/CliqueRule.h"

#include <lemon/connectivity.h>

#include <iostream>
#include <bitset>

namespace ysk {

class ClusterReductionInstance {
public:
  ClusterReductionInstance(ClusterEditingInstance* inst,
                           std::bitset<NUMBER_OF_REDUCTION_RULES> active,
                           double multiplicativeFactor,
                           bool conserveMultipleSolutions)
    : _lastSuccessRule(-1)
    , _currentRule(0)
    , _instance(inst)
    , _active(active)
    , _multiplicativeFactor(multiplicativeFactor)
    , _conserveMultipleSolutions(conserveMultipleSolutions)
  {
  }

  int applyReductionRules(yskLib::CplexInformer* informer);

  ClusterEditingInstance* getInstance();

  int computeConnectedComponents(WorkingCopyGraph::NodeMap<int>& connectedComps);

  void computeChildInstances(std::vector<ClusterReductionInstance*>& childInstances);

  bool operator < (const ClusterReductionInstance& c) const{ return (_instance->getSize() < c._instance->getSize());}
  
private:
  int _lastSuccessRule;
  int _currentRule;
  ClusterEditingInstance* _instance;
  std::bitset<NUMBER_OF_REDUCTION_RULES> _active;
  double _multiplicativeFactor;
  bool _conserveMultipleSolutions;
  
};

} // namespace ysk

#endif /* CLUSTERREDUCTIONINSTANCE_H */
