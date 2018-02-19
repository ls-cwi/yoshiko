#ifndef FPTREDUCTIONRULE_H
#define	FPTREDUCTIONRULE_H

#include "WorkingCopyInstance.h"

#include <string>

namespace ysk {
  
class FPTReductionRule {
public:
  FPTReductionRule(WorkingCopyInstance& inst,
                   std::string name,
                   bool conserveMultipleSolutions)
    : _success(false)
    , _instance(inst)
    , _name(name)
    , _conserveMultipleSolutions(conserveMultipleSolutions)
  {
  }
  
  virtual ~FPTReductionRule();
  
  virtual void apply() = 0;
  
  bool isSuccess();
  
  std::string getName();
  
protected:
  bool _success;
  WorkingCopyInstance& _instance;
  std::string _name;
  bool _conserveMultipleSolutions;
};
  
} // namespace ysk

#endif	/* FPTREDUCTIONRULE_H */

