#include "InducedCostsHeuristic.h"

using namespace std;
using namespace lemon;

namespace ysk {

void InducedCostsHeuristic::init(ParameterizedInstance& inst) {

	//Pretty much simply copies the old instance

  //Adjust a working copy with the same size
  _instance.init(countNodes(inst.getWorkingCopyInstance().getGraph()));

  WorkingCopyGraph::NodeMap<WorkingCopyGraph::Node> aref(_instance.getWorkingCopyInstance().getGraph());
  WorkingCopyGraph::NodeMap<WorkingCopyGraph::Node> bref(inst.getWorkingCopyInstance().getGraph());


  FullGraph::NodeIt x(_instance.getOrig());
  WorkingCopyGraph::NodeIt u(inst.getWorkingCopyInstance().getGraph());
  for(; u != INVALID; ++u, ++x) { //TODO: This is a while loop -> Codestyle
    _instance.initNode(x, inst.getWorkingCopyInstance().getNodeName(u), *(inst.getWorkingCopyInstance().getClusters()[u]));

    FullGraph::NodeIt y(_instance.getOrig(), x);
    WorkingCopyGraph::NodeIt v(inst.getWorkingCopyInstance().getGraph(), u);
    ++y;
    ++v;
    for(; v != INVALID; ++v, ++y) {
      _instance.initNode(y, inst.getWorkingCopyInstance().getNodeName(v), *(inst.getWorkingCopyInstance().getClusters()[v]));

      aref[x] = u;
      aref[y] = v;
      bref[u] = x;
      bref[v] = y;


      WorkingCopyGraph::Edge uv = inst.getWorkingCopyInstance().edge(u, v);
      FullGraph::Edge xy = _instance.getOrig().edge(x, y);
      _instance.initEdge(xy, inst.getWorkingCopyInstance().getWeight(uv), inst.getWorkingCopyInstance().getEdgeType(uv));

    }
  }
  _parameterizedInstance = new ParameterizedInstance(_instance.getWorkingCopyInstance(), aref, bref, inst);
}

void InducedCostsHeuristic::start() {

	//TODO: Document
  int i = 1;

  if (verbosity > 1)
    cout << "executing induced costs heuristic..."<<flush;

  while(true) {
	  //Killswitch
	  if (isTerminated){
		  break;
	  }
    if (verbosity > 1) {
      if(i % 10000 == 0)
        cout <<"."<<flush;
    }

    //Return the edge with the highest cost associated
    std::pair<WorkingCopyGraph::Edge, double> p = _parameterizedInstance->computeMax();


    if(p.first == INVALID) {
      break;
    }

    if(_parameterizedInstance->getICP(p.first) > _parameterizedInstance->getICF(p.first)) {
      _parameterizedInstance->setForbidden(p.first);
    } else {
      _parameterizedInstance->setPermanent(p.first);
    }
    i++;
  }

  for(WorkingCopyGraph::EdgeIt e(_parameterizedInstance->getWorkingCopyInstance().getGraph()); e != INVALID; ++e) {
    if(_parameterizedInstance->getWorkingCopyInstance().getWeight(e) != -numeric_limits<double>::infinity()) {
    	//Example where this CAN happen:
    	//Take a Triangle A B C, where AB and BC are permanent edges and AC is forbidden

      //cerr << "Fatal error: InducedCostsHeuristic: Heuristic was not applied exhaustively. This cannot happen."<<endl;
      //exit(-1);
    }
  }

  _upperBound = _instance.computeCost();

  if (verbosity > 1) {
    cout << " done." << endl;
    cout << "upper bound:\t" << _upperBound << endl;
    cout << "-------------------------" << endl;
  }
}

double InducedCostsHeuristic::getUpperBound() {
  return _upperBound;
}

double InducedCostsHeuristic::getSolution(ClusterEditingSolutions &solution) {
  solution.resize(1);
  solution.setSolution(0, _instance.getWorkingCopyInstance());
  return _upperBound;
}

InducedCostsHeuristic::~InducedCostsHeuristic() {
  delete _parameterizedInstance;
}

} // namespace ysk
