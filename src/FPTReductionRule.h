/* 
 * File:   FPTReductionRule.h
 * Author: emanuellaude
 *
 * Created on 26. August 2012, 12:14
 */

#ifndef FPTREDUCTIONRULE_H
#define	FPTREDUCTIONRULE_H

#include "WorkingCopyInstance.h"

#include <string>


class FPTReductionRule {
public:

    FPTReductionRule(WorkingCopyInstance& inst, std::string name, bool conserveMultipleSolutions) : _success(false), _instance(inst), _name(name), _conserveMultipleSolutions(conserveMultipleSolutions) {
    }
    
    virtual ~FPTReductionRule();
    
    virtual void apply() = 0;
    
    bool isSuccess();
    
    std::string getName();
private:
    
protected:
    bool _success;
    WorkingCopyInstance& _instance;
    std::string _name;
    bool _conserveMultipleSolutions;
};

#endif	/* FPTREDUCTIONRULE_H */

