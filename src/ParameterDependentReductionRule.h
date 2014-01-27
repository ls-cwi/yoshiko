//
//  ParameterDependentReductionRule.h
//  lemon-first-steps
//
//  Created by Emanuel Laude on 20.10.12.
//  Copyright (c) 2012 Emanuel Laude. All rights reserved.
//

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


class ParameterDependentReductionRule : public FPTReductionRule {
public:

    ParameterDependentReductionRule(WorkingCopyInstance& inst) : FPTReductionRule(inst, "Parameter Dependent Reduction Rule", true), _parameterizedInstance(_instance, -1) {
    }

    void apply();
    
private:
    ParameterizedInstance _parameterizedInstance;
};

#endif /* PARAMETERDEPENDENTREDUCTIONRULE_H */
