#ifndef CLIQUERULE_H
#define CLIQUERULE_H

#include <iostream>

#include "FPTReductionRule.h"
#include "WorkingCopyInstance.h"

namespace ysk {

class CliqueRule : public FPTReductionRule {
public:
  CliqueRule(WorkingCopyInstance& inst)
    : FPTReductionRule(inst, "Clique Rule", true)
  {
  }

  void apply();
};

} // namespace ysk

#endif /* CLIQUERULE_H */
