#include "Delta.h"

using namespace std;

namespace ysk {

double Delta::getValue(const WorkingCopyGraph::Node &node) {
  if(node == _u)
    return _deltaU;
  else if(node == _v)
    return _deltaV;
  else {
    cerr << "Fatal error: class Delta: trying to get value of non-end-node!"<<endl;
    exit(-1);
  }
}

void Delta::setDeltaU(WorkingCopyGraph::Node u, double deltaU) {
  _u = u;
  _deltaU = deltaU;
}

void Delta::setDeltaV(WorkingCopyGraph::Node v, double deltaV) {
  _v = v;
  _deltaV = deltaV;
}

} // namespace ysk
