#ifndef MERGINGRULE_H
#define	MERGINGRULE_H

#include <lemon/full_graph.h>
#include <lemon/adaptors.h>

#include "FPTReductionRule.h"
#include "WorkingCopyInstance.h"

namespace ysk {

class MergingRule : public FPTReductionRule {
public:
  
  MergingRule(WorkingCopyInstance& inst) : FPTReductionRule(inst, "Merging Rule", true) {
  }
  void apply();
private:
};
  
} // namespace ysk

#endif	/* MERGINGRULE_H */
