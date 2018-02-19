#include "FPTReductionRule.h"

using namespace std;

namespace ysk {

bool FPTReductionRule::isSuccess() {
  return _success;
}

string FPTReductionRule::getName() {
  return _name;
}

FPTReductionRule::~FPTReductionRule() {
}

} // namespace ysk
