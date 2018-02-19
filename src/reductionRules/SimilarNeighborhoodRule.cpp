#include "SimilarNeighborhoodRule.h"

using namespace std;
using namespace lemon;

namespace ysk {

//Runtime: O(V^3)
void SimilarNeighborhoodRule::init() {
  if (verbosity > 1)
    cout <<endl<<"initializing costs..."<<endl;

  int noEdges = countEdges(_instance.getGraph());

  int i = 1;
  int step = ceil((double) noEdges / 100.0);

  if (verbosity > 1)
    cout<<"\r0%"<<flush;

  for(WorkingCopyGraph::EdgeIt uv(_instance.getGraph()); uv != INVALID; ++uv, i++) {
    if (verbosity > 1) {
      if((i % step) == 0)
        cout<<"\r"<<(int) ((i / (double) noEdges)*100.0) <<"%"<<flush;
    }

    if(_instance.getWeight(uv)<= 0) {
      continue;
    }

    double upperBound = computeBounds(uv);
    _upperBound.push(uv, upperBound);
  }
  if (verbosity > 1) {
    cout <<"\rdone."<<endl;
  }
}

//Runtime: O(V)
double SimilarNeighborhoodRule::computeBounds(WorkingCopyGraph::Edge uv) {
  if(_instance.getWeight(uv)<= 0) {
    cerr << "Fatal error: SimilarNeighborhoodRule: cannot compute bounds of non-edge!"<<endl;
    exit(-1);
  }
  WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
  WorkingCopyGraph::Node v = _instance.getGraph().v(uv);

  double deltaU = 0.0;
  double deltaV = 0.0;
  double upperBound = 0.0;

  for(WorkingCopyGraph::NodeIt w(_instance.getGraph()); w != INVALID; ++w) {
    if(w == u || w == v) {
      continue;
    }

    WorkingCopyGraph::Edge uw = _instance.edge(u, w);
    WorkingCopyGraph::Edge vw = _instance.edge(v, w);

    if(_instance.getWeight(uw) > 0 && _instance.getWeight(vw) <= 0) {
      //w in N_u
      deltaU = deltaU + _instance.getWeight(uw);
      deltaV = deltaV -_instance.getWeight(vw);
    } else if(_instance.getWeight(uw) <= 0 && _instance.getWeight(vw) > 0) {
      //w in N_v
      deltaU = deltaU - _instance.getWeight(uw);
      deltaV = deltaV + _instance.getWeight(vw);
    } else {
      // w in W
      upperBound = upperBound + 0.5 * abs(_instance.getWeight(uw)-_instance.getWeight(vw));
    }
  }
  upperBound = upperBound + 0.5 * (deltaU +deltaV) -_instance.getWeight(uv);

  _delta[uv].setDeltaU(u, deltaU);
  _delta[uv].setDeltaV(v, deltaV);

  return upperBound;
}

//Runtime: O(log V)
void SimilarNeighborhoodRule::updateUpperBound(WorkingCopyGraph::Edge uv, double value) {
  if(_upperBound.state(uv) != MinHeap::IN_HEAP) {
    cerr << "Fatal error: SimilarNeighborhoodRule: updateUpperBound: edge "<<_instance.getEdgeName(uv)<< " not in heap. This cannot happen!"<<endl;
    exit(-1);
  }

  if(value == _upperBound[uv]) {
    return;
  }

  if(value > _upperBound[uv]) {
    _upperBound.increase(uv, value);
  } else {
    _upperBound.decrease(uv, value);
  }
}

//Runtime: O(V^2 log V)
void SimilarNeighborhoodRule::setPermanent(WorkingCopyGraph::Edge uv) {
  beforeSetPermanent(uv);
  WorkingCopyGraph::Node proxy = _instance.setPermanent(uv);

  afterSetPermanent(proxy);
}

//Rumtime: O(V^2 log V)
void SimilarNeighborhoodRule::afterSetPermanent(WorkingCopyGraph::Node proxy) {
  //compute new costs for all edges u'x incident to proxy u' after merge of uv
  for(WorkingCopyGraph::NodeIt x(_instance.getGraph()); x != INVALID; ++x) {
    if(x == proxy) {
      continue;
    }

    WorkingCopyGraph::Edge xp = _instance.edge(x, proxy);
    if(_upperBound.state(xp) == MinHeap::IN_HEAP) {
      cerr << "Fatal error: SimilarNeighborhoodRule: afterSetPermanent: edge "<<_instance.getEdgeName(xp)<< " in heap. This cannot happen!"<<endl;
      exit(-1);
    }
    if(_instance.getWeight(xp) <= 0) {
      continue;

    }


    double upperBound = computeBounds(xp);

    _upperBound.push(xp, upperBound);
  }
}

//Runtime: O(V^2 log V)
void SimilarNeighborhoodRule::beforeSetPermanent(WorkingCopyGraph::Edge uv) {
  WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
  WorkingCopyGraph::Node v = _instance.getGraph().v(uv);

  for(WorkingCopyGraph::NodeIt x(_instance.getGraph()); x != INVALID; ++x) {
    if(x == u || x == v) {
      continue;
    }

    WorkingCopyGraph::NodeIt y(_instance.getGraph(), x);
    ++y;

    for(; y != INVALID; ++y) {
      if(y == u || y == v) {
        continue;
      }



      //xy in E with xy not incident to u or v
      WorkingCopyGraph::Edge xy = _instance.edge(x, y);



      if(_instance.getWeight(xy) <= 0) {
        //only edges incident to u or v can switch from non-edge to edge => xy is not in the heap "upperBound"

        if(_upperBound.state(xy) == MinHeap::IN_HEAP) {
          cerr << "Fatal error: SimilarNeighborhoodRule: edge "<<_instance.getEdgeName(xy)<< " in heap. This cannot happen!"<<endl;
          exit(-1);
        }

        continue;
      }

      //"remove" old contribution to costs[xy] (before merge of uv)
      double deltaX = _delta[xy].getValue(x);
      double deltaY = _delta[xy].getValue(y);
      double upperBound = _upperBound[xy];

      //decrease upperBound by old deltas

      upperBound = upperBound - 0.5 * (deltaX + deltaY);

      WorkingCopyGraph::Node e[2];
      e[0] = u;
      e[1] = v;
      for(int i = 0; i < 2; i++) {
        WorkingCopyGraph::Node u = e[i];

        WorkingCopyGraph::Edge xu = _instance.edge(x, u);
        WorkingCopyGraph::Edge yu = _instance.edge(y, u);

        if(_instance.getWeight(xu) > 0 && _instance.getWeight(yu) <= 0) {
          //u in N_x
          deltaX = deltaX - _instance.getWeight(xu);
          deltaY = deltaY +_instance.getWeight(yu);
        } else if(_instance.getWeight(xu) <= 0 && _instance.getWeight(yu) > 0) {
          //u in N_y
          deltaX = deltaX + _instance.getWeight(xu);
          deltaY = deltaY - _instance.getWeight(yu);
        } else {
          // u in W
          upperBound = upperBound - 0.5 * abs(_instance.getWeight(xu)-_instance.getWeight(yu));

        }
      }

      //compute new contribution to costs[xy] (after merge of uv to u') and update costs
      WorkingCopyGraph::Edge xu = _instance.edge(x, u);
      WorkingCopyGraph::Edge xv = _instance.edge(x, v);
      double w1 = _instance.getWeight(xu) + _instance.getWeight(xv);

      WorkingCopyGraph::Edge yu = _instance.edge(y, u);
      WorkingCopyGraph::Edge yv = _instance.edge(y, v);
      double w2 = _instance.getWeight(yu) + _instance.getWeight(yv);

      if(w1 > 0 && w2 <= 0) {
        //u' in N_x
        deltaX = deltaX + w1;
        deltaY = deltaY - w2;
      } else if(w1 <= 0 && w2 > 0) {
        //u' in N_y
        deltaX = deltaX - w1;
        deltaY = deltaY + w2;
      } else {
        //u' in W
        upperBound = upperBound + 0.5 * abs(w1 - w2);
      }
      //save new deltas
      _delta[xy].setDeltaU(x, deltaX);
      _delta[xy].setDeltaV(y, deltaY);

      //increase upperBound by new deltas
      upperBound = upperBound + 0.5 * (deltaX + deltaY);

      //update upper bound. Runtime: O(log V)
      updateUpperBound(xy, upperBound);
    }


    //remove all "old" edges ux, vx with x in V from heap because there are invalid after removing u, v through merge operation.
    WorkingCopyGraph::Node e[2];
    e[0] = u;
    e[1] = v;
    for(int i = 0; i < 2; i++) {
      //Runtime: O(log V)
      WorkingCopyGraph::Node u = e[i];

      WorkingCopyGraph::Edge ux = _instance.edge(u, x);

      if(_instance.getWeight(ux) > 0 && _upperBound.state(ux) == MinHeap::IN_HEAP) {
        _upperBound.erase(ux);
      } else if(_instance.getWeight(ux) > 0 && _upperBound.state(ux) != MinHeap::IN_HEAP) {
        cerr << "Fatal error: SimilarNeighborhoodRule: edge "<<_instance.getEdgeName(ux)<< " not in heap as expected! This cannot happen!"<<endl;
        exit(-1);
      } else if(_instance.getWeight(ux) <= 0 && _upperBound.state(ux) == MinHeap::IN_HEAP) {
        cerr << "Fatal error: SimilarNeighborhoodRule: edge "<<_instance.getEdgeName(ux)<< " in heap. This cannot happen!"<<endl;
        exit(-1);
      } else {
        //everything OK!
      }

    }
  }
}

void SimilarNeighborhoodRule::setPermanent(vector<pair<WorkingCopyGraph::Edge, double> >* &skip) {
  WorkingCopyGraph::Edge uv = _upperBound.top();
  _upperBound.pop();

  for(vector<pair<WorkingCopyGraph::Edge, double> >::iterator it = skip->begin(); it != skip->end(); ++it) {
    _upperBound.push((*it).first, (*it).second);
  }
  delete skip;
  skip = new vector<pair<WorkingCopyGraph::Edge, double> >();
  setPermanent(uv);
  _success = true;
}

void SimilarNeighborhoodRule::apply() {
  init();
  vector<pair<WorkingCopyGraph::Edge, double> >* skip = new vector<pair<WorkingCopyGraph::Edge, double> >();


  if (verbosity > 1)
    cout <<"applying rule..."<<flush;

  int i = 1;

  while(_upperBound.size() > 0) {
    if (verbosity > 1) {
      if(i % 1000 == 0)
        cout <<"."<<flush;
    }
    WorkingCopyGraph::Edge uv = _upperBound.top();
    i++;

    if((_upperBound[uv] <= 0 && !_conserveMultipleSolutions) || (_upperBound[uv] < 0 && _conserveMultipleSolutions)) {
      setPermanent(skip);

      continue;
    }

    //c(uv) > lowerBound and c(uv) < upperBound => compute Dynamic Programming Table
    if(_instance.getWeight(uv)>computeLowerBound(uv)) {
      double maximum = computeMaximum(uv);
      if ((_c*_instance.getWeight(uv) >= maximum && !_conserveMultipleSolutions) || (_c *_instance.getWeight(uv) > maximum && _conserveMultipleSolutions)) {
        setPermanent(skip);

        continue;
      }
    }
    pair<WorkingCopyGraph::Edge, double> p;
    p.first = uv;
    p.second = _upperBound[uv];
    skip->push_back(p);
    _upperBound.pop();
  }
  delete skip;
}

double SimilarNeighborhoodRule::computeMaximum(WorkingCopyGraph::Edge uv) {
  WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
  WorkingCopyGraph::Node v = _instance.getGraph().v(uv);

  int bound = 0;
  int maxAbsWeight = 0;
  int k = 0;

  //foreach w in W
  for (WorkingCopyGraph::NodeIt w(_instance.getGraph()); w != INVALID; ++w) {
    if(w == u || w == v)
      continue;

    WorkingCopyGraph::Edge uw = _instance.edge(u, w);
    WorkingCopyGraph::Edge vw = _instance.edge(v, w);

    if(_instance.getWeight(uw) > 0 && _instance.getWeight(vw) <= 0) {
      // w in N_u
      continue;
    }

    if(_instance.getWeight(uw)<= 0 && _instance.getWeight(vw) > 0) {
      // w in N_v
      continue;
    }

    //w in W because N_u, N_v and W is a partition of V

    k++;
    double weight = ceil(abs(_c * _instance.getWeight(uw)));
    //What to do if weight is infinit (edge forbidden)? To be on the save side return infinity to prevent merge.
    if(weight == std::numeric_limits<double>::infinity()) {
      return std::numeric_limits<double>::infinity();
    }

    bound += (int) weight;
    if (weight > maxAbsWeight)
      maxAbsWeight = weight;
  }

  DynProgTable* table = new DynProgTable(bound + maxAbsWeight);

  for (int x = -(bound + maxAbsWeight); x <= (bound + maxAbsWeight); x++) {
    table->setValue(x, -std::numeric_limits<double>::infinity());
  }

  table->setValue(0, 0);
  table->swap();


  int i =1;
  double maximum = 0;
  for (WorkingCopyGraph::NodeIt w(_instance.getGraph()); w != INVALID; ++w) {
    if(w == u || w == v)
      continue;

    WorkingCopyGraph::Edge uw = _instance.edge(u, w);
    WorkingCopyGraph::Edge vw = _instance.edge(v, w);

    if(_instance.getWeight(uw) > 0 && _instance.getWeight(vw) <= 0) {
      // w in N_u
      continue;
    }

    if(_instance.getWeight(uw)<= 0 && _instance.getWeight(vw) > 0) {
      // w in N_v
      continue;
    }

    //w in W because N_u, N_v and W is a partition of V

    ++i;

    for (int x = -(bound + maxAbsWeight); x < -bound; x++) {
      table->setValue(x, -std::numeric_limits<double>::infinity());
    }

    for (int x = bound + 1; x <= (bound + maxAbsWeight); x++) {
      table->setValue(x, -std::numeric_limits<double>::infinity());
    }

    for (int x = -bound; x <= bound; x++) {

      double value = max(table->getValue(x),
                         table->getValue(x + (int) floor(_c*_instance.getWeight(uw))) + (int) ceil(_c*_instance.getWeight(vw)));
      value = max(value, table->getValue(x - (int) ceil(_c*_instance.getWeight(uw))) - (int) floor(_c*_instance.getWeight(vw)));
      if(i != k) {
        table->setValue(x, value);
      } else {
        double minimum = min(x + _c*_delta[uv].getValue(u), value + _c*_delta[uv].getValue(v));
        if (maximum < minimum) {
          maximum = minimum;
        }
      }

    }

    if(i != k) {
      table->swap();
    }
  }

  delete table;
  return maximum;
}

double SimilarNeighborhoodRule::computeLowerBound(WorkingCopyGraph::Edge uv) {
  WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
  WorkingCopyGraph::Node v = _instance.getGraph().v(uv);

  return min(_delta[uv].getValue(u), _delta[uv].getValue(v));
}

} // namespace ysk
