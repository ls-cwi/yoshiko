/* 
 * File:   FPTReductionRule.cpp
 * Author: emanuellaude
 * 
 * Created on 26. August 2012, 12:14
 */

#include "FPTReductionRule.h"

using namespace std;

bool FPTReductionRule::isSuccess() {
    return _success;
}

string FPTReductionRule::getName() {
    return _name;
}

FPTReductionRule::~FPTReductionRule() {
    
}
