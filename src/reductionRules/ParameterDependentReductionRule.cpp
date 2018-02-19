#include "ParameterDependentReductionRule.h"

using namespace std;
using namespace lemon;

namespace ysk {

void ParameterDependentReductionRule::apply() {
  _parameterizedInstance.init();
  InducedCostsHeuristic* inducedCostsHeuristic = new InducedCostsHeuristic(_parameterizedInstance);
  inducedCostsHeuristic->start();

  _parameterizedInstance.setParameter(inducedCostsHeuristic->getUpperBound());

  delete inducedCostsHeuristic;

  int i = 1;
  if (verbosity > 1)
    cout << "applying rule..." << flush;

  while(true) {
	  //Killswitch
	  if (isTerminated){
		  break;
	  }
    if (verbosity > 1) {
      if (i % 10000 == 0)
        cout << "." << flush;
    }

    std::pair<WorkingCopyGraph::Edge, double> p = _parameterizedInstance.computeMax();


    if (p.first == INVALID) {
      break;
    }

    if (p.second > _parameterizedInstance.getParameter() + eps) {
      if (_parameterizedInstance.getICP(p.first) +_parameterizedInstance.getLowerBound() > _parameterizedInstance.getParameter() + eps && _parameterizedInstance.getICF(p.first) + _parameterizedInstance.getLowerBound() > _parameterizedInstance.getParameter()+eps) {

        cerr << "Fatal Error: icp and icf both greater than k. => There exists no solution. This cannot happen since k was initialized with an upper bound!" << endl;
        cerr << "icp[" << _instance.getEdgeName(p.first)<<"] = "<< _parameterizedInstance.getICP(p.first) + _parameterizedInstance.getLowerBound() << endl;
        cerr << "icf[" << _instance.getEdgeName(p.first)<<"] = " << _parameterizedInstance.getICF(p.first) + _parameterizedInstance.getLowerBound() << endl;
        cerr <<"k = "<<_parameterizedInstance.getParameter() << endl;
        exit(-1);
      }

      _success = true;

      if(_parameterizedInstance.getICP(p.first) > _parameterizedInstance.getICF(p.first)) {
        _parameterizedInstance.setForbidden(p.first);
      } else {
        _parameterizedInstance.setPermanent(p.first);
      }
    } else {
      break;
    }
    i++;
  }
}

} // namespace ysk
