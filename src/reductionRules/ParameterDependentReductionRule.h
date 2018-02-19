#ifndef PARAMETERDEPENDENTREDUCTIONRULE_H
#define PARAMETERDEPENDENTREDUCTIONRULE_H

#include <iostream>
#include <cmath>

#include <lemon/adaptors.h>

#include "FPTReductionRule.h"
#include "WorkingCopyInstance.h"
#include "ParameterizedInstance.h"
#include "InducedCostsHeuristic.h"
#include "Globals.h"

namespace ysk {

class ParameterDependentReductionRule : public FPTReductionRule {
public:
  
  ParameterDependentReductionRule(WorkingCopyInstance& inst) : FPTReductionRule(inst, "Parameter Dependent Reduction Rule", true), _parameterizedInstance(_instance, -1) {
  }
  
  void apply();
  
private:
  ParameterizedInstance _parameterizedInstance;
};
  
} // namespace ysk

#endif /* PARAMETERDEPENDENTREDUCTIONRULE_H */
