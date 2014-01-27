/* 
 * File:   MergingRule.h
 * Author: emanuellaude
 *
 * Created on 27. August 2012, 13:06
 */

#ifndef MERGINGRULE_H
#define	MERGINGRULE_H

#include <lemon/full_graph.h>
#include <lemon/adaptors.h>

#include "FPTReductionRule.h"
#include "WorkingCopyInstance.h"


class MergingRule : public FPTReductionRule {
public:

    MergingRule(WorkingCopyInstance& inst) : FPTReductionRule(inst, "Merging Rule", true) {
    }
    void apply();
private:
};

#endif	/* MERGINGRULE_H */

