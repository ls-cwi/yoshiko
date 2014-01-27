//
//  CliqueRule.h
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 20.11.12.
//  Copyright (c) 2012 Emanuel Laude. All rights reserved.
//

#ifndef CLIQUERULE_H
#define CLIQUERULE_H

#include <iostream>

#include "FPTReductionRule.h"
#include "WorkingCopyInstance.h"

class CliqueRule : public FPTReductionRule {
public:
    CliqueRule(WorkingCopyInstance& inst) : FPTReductionRule(inst, "Clique Rule", true) {
        
    }
    
    void apply();
private:
};

#endif /* CLIQUERULE_H */
