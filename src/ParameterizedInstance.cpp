#include "ParameterizedInstance.h"

using namespace std;
using namespace lemon;

namespace ysk {

ParameterizedInstance::~ParameterizedInstance() {
//   for(WorkingCopyGraph::NodeIt u(_instance.getGraph()); u != INVALID; ++u) {
//     delete _icpCrossRef[u];
//     delete _icp[u];
// 
//     delete _icfCrossRef[u];
//     delete _icf[u];
//   }
// 
//   for(vector<WorkingCopyGraph::Node*>::iterator uvw = _conflictTriples.begin(); uvw != _conflictTriples.end(); ++uvw) {
//     delete [] *uvw;
//   }
}

WorkingCopyInstance& ParameterizedInstance::getWorkingCopyInstance() {
  return _instance;
}

double ParameterizedInstance::getParameter() {
  return _parameter;
}

void ParameterizedInstance::setParameter(double parameter) {
  _parameter = parameter;
}

std::pair<WorkingCopyGraph::Node, WorkingCopyGraph::Node> ParameterizedInstance::getNodesOrderById(const WorkingCopyGraph::Edge &uv) {
  std::pair<WorkingCopyGraph::Node, WorkingCopyGraph::Node> nodes;
  nodes.first = _instance.getGraph().nodeFromId(min(_instance.getGraph().id(_instance.getGraph().u(uv)), _instance.getGraph().id(_instance.getGraph().v(uv))));
  nodes.second = _instance.getGraph().oppositeNode(nodes.first, uv);

  return nodes;
}

double ParameterizedInstance::getICP(const WorkingCopyGraph::Edge &uv) {
  std::pair<WorkingCopyGraph::Node, WorkingCopyGraph::Node> nodes = getNodesOrderById(uv);

  return (*_icp[nodes.first])[nodes.second];
}

double ParameterizedInstance::getICF(const WorkingCopyGraph::Edge &uv) {
  std::pair<WorkingCopyGraph::Node, WorkingCopyGraph::Node> nodes = getNodesOrderById(uv);

  return (*_icf[nodes.first])[nodes.second];
}

void ParameterizedInstance::setICP(const WorkingCopyGraph::Edge &uv, double value) {
  std::pair<WorkingCopyGraph::Node, WorkingCopyGraph::Node> nodes = getNodesOrderById(uv);

  _icp[nodes.first]->push(nodes.second, value);
}

void ParameterizedInstance::setICF(const WorkingCopyGraph::Edge &uv, double value) {
  std::pair<WorkingCopyGraph::Node, WorkingCopyGraph::Node> nodes = getNodesOrderById(uv);

  _icf[nodes.first]->push(nodes.second, value);
}

void ParameterizedInstance::updateICP(const WorkingCopyGraph::Edge &uv, double value) {
  std::pair<WorkingCopyGraph::Node, WorkingCopyGraph::Node> nodes = getNodesOrderById(uv);

  if(value == (*_icp[nodes.first])[nodes.second]) {
    return;
  }

  if(value > (*_icp[nodes.first])[nodes.second]) {
    _icp[nodes.first]->decrease(nodes.second, value);
  } else {
    _icp[nodes.first]->increase(nodes.second, value);
  }
}

void ParameterizedInstance::updateICF(const WorkingCopyGraph::Edge &uv, double value) {
  std::pair<WorkingCopyGraph::Node, WorkingCopyGraph::Node> nodes = getNodesOrderById(uv);


  if(value == (*_icf[nodes.first])[nodes.second]) {
    return;
  }

  if(value > (*_icf[nodes.first])[nodes.second]) {
    _icf[nodes.first]->decrease(nodes.second, value);
  } else {
    _icf[nodes.first]->increase(nodes.second, value);
  }
}

void ParameterizedInstance::removeICP(const WorkingCopyGraph::Edge &uv) {
  std::pair<WorkingCopyGraph::Node, WorkingCopyGraph::Node> nodes = getNodesOrderById(uv);

  if(_icp[nodes.first]->state(nodes.second) == MaxHeap::IN_HEAP) {
    _icp[nodes.first]->erase(nodes.second);
  }
}

void ParameterizedInstance::removeICF(const WorkingCopyGraph::Edge &uv) {
  std::pair<WorkingCopyGraph::Node, WorkingCopyGraph::Node> nodes = getNodesOrderById(uv);

  if(_icf[nodes.first]->state(nodes.second) == MaxHeap::IN_HEAP) {
    _icf[nodes.first]->erase(nodes.second);
  }
}

std::pair<WorkingCopyGraph::Edge, double> ParameterizedInstance::computeMax() {
  double max = 0.0;
  WorkingCopyGraph::Node u = INVALID;
  WorkingCopyGraph::Node v = INVALID;

  //Loop over all nodes
  for(WorkingCopyGraph::NodeIt w(_instance.getGraph()); w != INVALID; ++w) {
	  //Eventually we want to maximize so -inf is the initial value
    double icp = -numeric_limits<double>::infinity();
    if(_icp[w]->size() >= 1) { //There are already values on the heap
      icp = (*_icp[w])[_icp[w]->top()] +_lbValue;
    }

    double icf = -numeric_limits<double>::infinity();
    if(_icf[w]->size() >= 1) {
      icf = (*_icf[w])[_icf[w]->top()] + _lbValue;
    }

    if(icf >= max || icp >= max) {
      u = w;
      if(icp > icf) {
        max = icp;
        v = _icp[w]->top();
      } else {
        max = icf;
        v = _icf[w]->top();
      }
    }
  }

  //Create the pair as a return value
  std::pair<WorkingCopyGraph::Edge, double> p;
  if(u != INVALID && v != INVALID) {
    p.first = _instance.edge(u, v);
  } else {
    p.first = INVALID;
  }
  p.second = max;

  return p;
}

void ParameterizedInstance::init() {
  computeLowerBound();

  if (verbosity > 1)
    cout << "initializing induced costs..."<<endl;

  int noEdges = countEdges(_instance.getGraph());
  int i = 1;
  int step = ceil((double) noEdges / 100.0);

  if (verbosity > 1)
    cout<<"\r0%"<<flush;

  for(WorkingCopyGraph::NodeIt u(_instance.getGraph()); u != INVALID; ++u) {
    WorkingCopyGraph::NodeIt v(_instance.getGraph(), u);
    ++v;
    for(; v != INVALID; ++v) {
      if (verbosity > 1) {
        if((i % step) == 0)
          cout<<"\r"<<(int) ((i / (double) noEdges)*100.0) <<"%"<<flush;
      }

      WorkingCopyGraph::Edge uv = _instance.edge(u, v);
      i++;
      if(_instance.isForbidden(uv) || _instance.isPermanent(uv))
        continue;

      double icp = 0.0;
      double icf = 0.0;
      computeInducedCosts(uv, icp, icf);

      setICP(uv, icp);
      setICF(uv, icf);
    }
  }
  if (verbosity > 1) {
    cout <<"\rdone."<<endl;
    cout<<"-------------------------"<<endl;
  }
}

void ParameterizedInstance::init(WorkingCopyGraph::NodeMap<WorkingCopyGraph::Node> &aref, WorkingCopyGraph::NodeMap<WorkingCopyGraph::Node> &bref, ParameterizedInstance &paramInst) {
  _lbValue = paramInst._lbValue;

  for(WorkingCopyGraph::NodeIt u(_instance.getGraph()); u != INVALID; ++u) {
    WorkingCopyGraph::NodeIt v(_instance.getGraph(), u);
    ++v;
    for(; v != INVALID; ++v) {
      _nodesLowerBound[u] = paramInst._nodesLowerBound[aref[u]];
      _nodesLowerBound[v] = paramInst._nodesLowerBound[aref[v]];
      WorkingCopyGraph::Edge uv = _instance.edge(u, v);
      WorkingCopyGraph::Edge xy = paramInst._instance.edge(aref[u], aref[v]);
      _edgesLowerBound[uv] = paramInst._edgesLowerBound[xy];
      _lbDecrease[uv] = paramInst._lbDecrease[xy];

      if(_edgesLowerBound[uv]>0) {
        WorkingCopyGraph::Node* xyz = paramInst._conflictTriple[xy];
        _conflictTriple[uv] = new WorkingCopyGraph::Node[3];
        _conflictTriples.push_back(_conflictTriple[uv]);
        _conflictTriple[uv][0] = bref[xyz[0]];
        _conflictTriple[uv][1] = bref[xyz[1]];
        _conflictTriple[uv][2] = bref[xyz[2]];
      }

      if(!_instance.isForbidden(uv) && !_instance.isPermanent(uv)) {
        setICP(uv, paramInst.getICP(xy));
        setICF(uv, paramInst.getICF(xy));
      }
    }
  }
}

void ParameterizedInstance::computeInducedCosts(const WorkingCopyGraph::Edge& uv, double& icp, double& icf) {

  icp = max(-1*_instance.getWeight(uv), 0.0);
  icf = max(_instance.getWeight(uv), 0.0);

  WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
  WorkingCopyGraph::Node v = _instance.getGraph().v(uv);

  for (WorkingCopyGraph::NodeIt w(_instance.getGraph()); w != INVALID; ++w) {
    if(w != u && w != v) {
      WorkingCopyGraph::Edge uw = _instance.edge(u, w);
      WorkingCopyGraph::Edge vw = _instance.edge(v, w);

      //w element of N(u) and w element of N(v)
      if(_instance.getWeight(uw) > 0 && _instance.getWeight(vw) > 0) {
        icf += min(_instance.getWeight(uw), _instance.getWeight(vw));
      }

      //w element of (N(u) "union" N(v)) "minus" (N(u) "intersection" N(v))
      if((_instance.getWeight(uw) > 0 && _instance.getWeight(vw) < 0) || (_instance.getWeight(uw) < 0 && _instance.getWeight(vw) > 0)) {
        icp += min(abs(_instance.getWeight(uw)), abs(_instance.getWeight(vw)));
      }
    }
  }


  _lbDecrease[uv] = getLBDecrease(uv);
  icp = icp + _lbDecrease[uv];
  icf = icf + _lbDecrease[uv];
}


double ParameterizedInstance::getLBDecrease(const WorkingCopyGraph::Edge& uv) {
  double decrease = 0.0;
  WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
  WorkingCopyGraph::Node v = _instance.getGraph().v(uv);

  decrease = -_nodesLowerBound[u] - _nodesLowerBound[v] + _edgesLowerBound[uv];


  return decrease;
}

void ParameterizedInstance::updateLowerBound(const WorkingCopyGraph::Edge &uv, double value) {

  _lbValue = _lbValue - _edgesLowerBound[uv] + value;

  WorkingCopyGraph::Node* uvw = _conflictTriple[uv];

  for(int i = 0; i < 3; i++) {
    _nodesLowerBound[uvw[i]] = _nodesLowerBound[uvw[i]] - _edgesLowerBound[uv] +value;
  }

  for(int k = 0; k < 2; k++) {
    for(int l = k+1; l <3; l++) {
      WorkingCopyGraph::Edge xy = _instance.edge(uvw[l], uvw[k]);
      _edgesLowerBound[xy] = value;
    }
  }
}

void ParameterizedInstance::setForbidden(const WorkingCopyGraph::Edge& uv) {
  beforeSetForbidden(uv);

  _instance.setForbidden(uv);
}

void ParameterizedInstance::updateLowerBoundBeforeSetForbidden(const WorkingCopyGraph::Edge &uv, vector<WorkingCopyGraph::Node*>& cts) {

  //flag to indicate, that a conflict triple was invalidated => trigger search for new cts
  bool invalidatedCT = false;

  //Check if uv is part of a conflict triple
  if(_edgesLowerBound[uv]> 0) {
    //Edge uv is part of a conflict triple

    if(_instance.getWeight(uv)>0) {
      //s(uv) > 0
      invalidatedCT = true;

      //corresponding conflict triple uvw will be destroyed => uvw does no longer contribute to lower bound => update lower bound
      updateLowerBound(uv, 0.0);
    } else {
      //s(uv) < 0

      //corresponding conflict triple may become more expensive because weight of non edge part of conflict triple is set to infinity => contribution to lower bound is minimum of the weight of the other two edges
      //compute minimum of cost of the other two edges
      double cost = std::numeric_limits<double>::infinity();
      WorkingCopyGraph::Node* uvw = _conflictTriple[uv];

      for(int k = 0; k < 2; k++) {
        for(int l = k+1; l <3; l++) {
          WorkingCopyGraph::Edge xy = _instance.edge(uvw[l], uvw[k]);
          if(xy != uv) {
            if(abs(_instance.getWeight(xy))<cost) {
              cost = _instance.getWeight(xy);
            }
          }
        }
      }

      if(cost != _edgesLowerBound[uv]) {
        updateLowerBound(uv, cost);
      }
    }
  }

  //compute new conflict triples
  WorkingCopyGraph::EdgeMap<int> crossRef(_instance.getGraph(), -1);
  BinHeap<double, WorkingCopyGraph::EdgeMap<int>, std::greater<double> > edgesOrderByWeight(crossRef);
  WorkingCopyGraph::EdgeMap<double> weight(_instance.getGraph(), 0.0);
  WorkingCopyGraph::EdgeMap<bool> candidate(_instance.getGraph(), false);

  if(invalidatedCT) {
    //ct containing uv was destroyed
    WorkingCopyGraph::Node* uvw = _conflictTriple[uv];

    //There is the chance of new conflict triples containing one of the edges of old ct uvw.
    //Add edges uv, uw, vw to edgesOrderByWeight to ensure that all 3 chances of a new ct are checked.
    for(int k = 0; k < 2; k++) {
      for(int l = k+1; l <3; l++) {
        WorkingCopyGraph::Edge xy = _instance.edge(uvw[l], uvw[k]);
        candidate[xy] =true;
        if(xy == uv) {
          //uv has weight -INF now
          edgesOrderByWeight.push(xy, numeric_limits<double>::infinity());
          weight[xy] = -numeric_limits<double>::infinity();
        } else {
          edgesOrderByWeight.push(xy, abs(_instance.getWeight(xy)));
          weight[xy] = _instance.getWeight(xy);
        }
      }
    }
  } else if(_edgesLowerBound[uv]<= 0 && _instance.getWeight(uv)>0) {
    //uv was not part of a conflict triple. But now edge uv was deleted and c(uv) changed from ? > 0 to -INF < 0. => There is the chance of a new conflict triple with edge uv.
    edgesOrderByWeight.push(uv, numeric_limits<double>::infinity());
    weight[uv] = -numeric_limits<double>::infinity();
    candidate[uv] = true;
  }

  computeLowerBound(uv, edgesOrderByWeight, weight, candidate, cts);
}

//Runtime: O(log n)
void ParameterizedInstance::updateLBDecrease(const WorkingCopyGraph::Edge &uv) {
  if(_instance.isForbidden(uv) || _instance.isPermanent(uv)) {
    return;
  }

  //icf(uv)
  double icf = getICF(uv);
  icf = icf - _lbDecrease[uv] + getLBDecrease(uv);
  updateICF(uv, icf);

  //icp(uv)
  double icp = getICP(uv);
  icp = icp - _lbDecrease[uv] + getLBDecrease(uv);
  updateICP(uv, icp);


  _lbDecrease[uv] = getLBDecrease(uv);
}

//Runtime: O(n log n)
void ParameterizedInstance::beforeSetForbidden(const WorkingCopyGraph::Edge& uv) {
  WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
  WorkingCopyGraph::Node v = _instance.getGraph().v(uv);

  //update lower bound. Reason: setting edge uv to forbidden can trigger the destruction of the corresponding conflict triple uvw if s(uv) > 0.
  //Therefore there can arise a new ct containing one of the edges uv, uw, vw.
  //Otherwise it can trigger the increase of the cost to solve the conflict triple.

  //Old ct
  WorkingCopyGraph::Node* uvw;
  set<WorkingCopyGraph::Node> update;

  if(_edgesLowerBound[uv] > 0) {
    uvw = _conflictTriple[uv];

    for(int i = 0; i < 3; i++) {
      WorkingCopyGraph::Node w = uvw[i];
      if(w != u && w != v) {
        update.insert(w);
      }
    }
  }

  //new conflict triples.
  vector<WorkingCopyGraph::Node*> cts;
  updateLowerBoundBeforeSetForbidden(uv, cts); //Runtime: (O(V))

  for(size_t j= 0; j <cts.size(); j++) {
    //There is at least one new ct abc. Add nodes of new ct to list "update".

    WorkingCopyGraph::Node* abc = cts[j];

    for(int i = 0; i < 3; i++) {
      WorkingCopyGraph::Node a = abc[i];
      if(a != u && a != v) {
        update.insert(a);
      }
    }
  }

  //Update internal all lb values of edges ab, ac, bc of abc[0..2] as well as aw with a in {a, b, c} except xy with x in {u, v} and y in {a, b, c}.
  for(set<WorkingCopyGraph::Node>::iterator a = update.begin(); a != update.end(); ++a) {
    set<WorkingCopyGraph::Node>::iterator b = update.find(*a);
    ++b;
    for(; b != update.end(); ++b) {
      WorkingCopyGraph::Edge ab = _instance.edge(*a, *b);
      updateLBDecrease(ab);
    }
  }

  //update parameter k
  _parameter = _parameter - max(0.0, _instance.getWeight(uv));

  for(WorkingCopyGraph::NodeIt x(_instance.getGraph()); x != INVALID; ++x) {
    if(x == u || x == v) {
      continue;
    }

    //There was a conflict triple uvw that was destroyed through setForbidden(uv) and the arise of at least one and at most 3 new cts abc was triggered.
    //update all lbDecrease, icp and icf values that are affected. Set contains at most 4 nodes: {a, b, c, w} =>
    //Runtime: O(log n)
    for(set<WorkingCopyGraph::Node>::iterator a = update.begin(); a != update.end(); ++a) {
      if(update.find(x) == update.end()) {
        WorkingCopyGraph::Edge ax = _instance.edge(*a, x);
        updateLBDecrease(ax);
      }
    }

    WorkingCopyGraph::Node e[2];
    e[0] = u;
    e[1] = v;

    //update icp and icf since edge uv has been set to forbidden.
    for(int i = 0; i < 2; i++) {
      WorkingCopyGraph::Node u = e[i];
      WorkingCopyGraph::Node v = e[(i+1)%2];

      WorkingCopyGraph::Edge vx = _instance.edge(v, x);
      WorkingCopyGraph::Edge ux = _instance.edge(u, x);

      if(_instance.isForbidden(ux) || _instance.isPermanent(ux))
        continue;

      //icf(ux)
      double icf = getICF(ux);
      if(_instance.getWeight(uv) > 0 && _instance.getWeight(vx) > 0) {
        icf = icf - min(_instance.getWeight(uv), _instance.getWeight(vx));

      }
      icf = icf - _lbDecrease[ux] + getLBDecrease(ux);
      updateICF(ux, icf);

      //icp(ux)
      double icp = getICP(ux);
      if(_instance.getWeight(uv)>0) {
        if(_instance.getWeight(vx) <= 0) {
          icp = icp - min(abs(_instance.getWeight(uv)), abs(_instance.getWeight(vx)));
        } else {
          icp = icp + _instance.getWeight(vx);
        }
      } else {
        if(_instance.getWeight(vx)>0) {
          icp = icp - min(abs(_instance.getWeight(uv)), abs(_instance.getWeight(vx))) + _instance.getWeight(vx);
        }
      }
      icp = icp - _lbDecrease[ux] + getLBDecrease(ux);
      updateICP(ux, icp);
      _lbDecrease[ux] = getLBDecrease(ux);
    }
  }


  removeICP(uv);
  removeICF(uv);
}

void ParameterizedInstance::setPermanent(const WorkingCopyGraph::Edge &uv) {
  WorkingCopyGraph::Node proxy = _instance.getGraph().u(uv);
  WorkingCopyGraph::Node del = _instance.getGraph().v(uv);

  beforeSetPermanent(uv, proxy);

  delete _icp[del];
  delete _icpCrossRef[del];
  delete _icf[del];
  delete _icfCrossRef[del];

  //set permanent uv => merge nodes u, v
  _instance.setPermanent(uv, proxy);

  afterSetPermanent(uv, proxy);
}

void ParameterizedInstance::updateLowerBoundBeforeSetPermanent(const WorkingCopyGraph::Edge &uv, const WorkingCopyGraph::Node& proxy) {
  WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
  WorkingCopyGraph::Node v = _instance.getGraph().v(uv);

  WorkingCopyGraph::Node e[2];
  e[0] = u;
  e[1] = v;

  WorkingCopyGraph::EdgeMap<int> crossRef(_instance.getGraph(), -1);
  BinHeap<double, WorkingCopyGraph::EdgeMap<int>, std::greater<double> > edgesOrderByWeight(crossRef);
  WorkingCopyGraph::EdgeMap<double> weight(_instance.getGraph(), 0.0);
  WorkingCopyGraph::EdgeMap<bool> candidate(_instance.getGraph(), false);

  //update/destroy old conflict triples that are triggered to become invalid
  for(WorkingCopyGraph::NodeIt w(_instance.getGraph()); w != INVALID; ++w) {
    if(u == w || v == w)
      continue;

    //set up heap with edges u'w with key c(u'w)=c(uw)+c(vw)
    WorkingCopyGraph::Edge pw = _instance.edge(proxy, w);
    weight[pw] = _instance.getWeight(_instance.edge(u, w)) + _instance.getWeight(_instance.edge(v, w));
    candidate[pw] = true;
    edgesOrderByWeight.push(pw, abs(weight[pw]));

    //update lowerBound
    for(int i = 0; i < 2; i++) {

      WorkingCopyGraph::Edge uw = _instance.edge(e[i], w);

      //check if uw is part of a conflict triple
      if(_edgesLowerBound[uw] > 0) {
        //yes => corresponding conflict triple uvw was destroyed (maybe) => uvw does no longer contribute to lower bound => update lower bound

        updateLowerBound(uw, 0.0);
      }
    }
  }
  vector<WorkingCopyGraph::Node*> cts;
  computeLowerBound(uv, edgesOrderByWeight, weight, candidate, cts);
}

void ParameterizedInstance::computeLowerBound(const WorkingCopyGraph::Edge &uv, BinHeap<double, WorkingCopyGraph::EdgeMap<int>, std::greater<double> > &edgesOrderByWeight, WorkingCopyGraph::EdgeMap<double>& weight, WorkingCopyGraph::EdgeMap<bool>& candidate, vector<WorkingCopyGraph::Node*>& cts) {


  WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
  WorkingCopyGraph::Node v = _instance.getGraph().v(uv);


  //iterate over all edges u'w and look for NEW conflict triples. (Runtime: beforeSetPermanent => O(V^2) / beforeSetForbidden => O(V))
  int size = edgesOrderByWeight.size();
  for(int i = 0; i < size; ++i) {
    WorkingCopyGraph::Edge xy = edgesOrderByWeight.top();
    edgesOrderByWeight.pop();

    if(_edgesLowerBound[xy] > 0) {
      continue;
    }

    WorkingCopyGraph::Node x = _instance.getGraph().u(xy);
    WorkingCopyGraph::Node y = _instance.getGraph().v(xy);

    WorkingCopyGraph::Node z = INVALID;
    WorkingCopyGraph::Edge xz = INVALID;
    WorkingCopyGraph::Edge yz = INVALID;


    double cost = 0.0;
    for(WorkingCopyGraph::NodeIt node(_instance.getGraph()); node != INVALID; ++node) {
      //nodes u, v may get merged
      if(node == x || node == y || node == u || node == v) {
        continue;
      }

      WorkingCopyGraph::Edge e1 = _instance.edge(x, node);
      WorkingCopyGraph::Edge e2 = _instance.edge(y, node);


      //edges e1 or e2 already part of an existing conflict triple => skip
      if(_edgesLowerBound[e1]>0 || _edgesLowerBound[e2]>0) {
        continue;
      }


      double w1 = _instance.getWeight(e1);
      if(candidate[e1]) {
        w1 = weight[e1];
      }
      double w2 = _instance.getWeight(e2);
      if(candidate[e2]) {
        w2 = weight[e2];
      }


      if(weight[xy] > 0) {
        if((w1 > 0 && w2 <= 0) || (w1 <= 0 && w2 > 0)) {
          //conflict triple found
          z = node;
          xz = e1;
          yz = e2;
          cost = min(min(weight[xy], abs(w1)), abs(w2));
          break;
        }
      } else {
        if(w1 > 0 && w2 > 0) {
          //conflict triple found
          z = node;
          xz = e1;
          yz = e2;
          cost = min(min(-weight[xy], w1), w2);
          break;
        }
      }
    }

    if(z != INVALID) {
      //xyz is a conflict triple. Its edges are not used by other conflict triples.

      WorkingCopyGraph::Node* xyz = new WorkingCopyGraph::Node[3];
      xyz[0] =x;
      xyz[1] =y;
      xyz[2] =z;

      _lbValue += cost;
      _nodesLowerBound[x] += cost;
      _nodesLowerBound[y] += cost;
      _nodesLowerBound[z] += cost;



      _edgesLowerBound[xy] = cost;
      _edgesLowerBound[xz] = cost;
      _edgesLowerBound[yz] = cost;

      _conflictTriple[xy] = xyz;
      _conflictTriple[xz] = xyz;
      _conflictTriple[yz] = xyz;

      _conflictTriples.push_back(xyz);
      cts.push_back(xyz);
    }
  }
}

void ParameterizedInstance::beforeSetPermanent(const WorkingCopyGraph::Edge& uv, const WorkingCopyGraph::Node& proxy) {
  WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
  WorkingCopyGraph::Node v = _instance.getGraph().v(uv);

  _parameter = _parameter - (getICP(uv) - _lbDecrease[uv]);


  WorkingCopyGraph::Node e[2];
  e[0] = u;
  e[1] = v;

  updateLowerBoundBeforeSetPermanent(uv, proxy);

  for(WorkingCopyGraph::NodeIt w(_instance.getGraph()); w != INVALID; ++w) {
    if(u == w || v == w)
      continue;

    WorkingCopyGraph::NodeIt x(_instance.getGraph(), w);
    ++x;
    for(; x != INVALID; ++x) {
      if(u == x || v == x)
        continue;

      WorkingCopyGraph::Edge wx = _instance.edge(w, x);

      if(_instance.isForbidden(wx) || _instance.isPermanent(wx)) {
        continue;
      }

      double icf = getICF(wx) - _lbDecrease[wx] + getLBDecrease(wx);
      double icp = getICP(wx) - _lbDecrease[wx] + getLBDecrease(wx);
      _lbDecrease[wx] = getLBDecrease(wx);

      for(int i = 0; i < 2; i++) {
        WorkingCopyGraph::Node u = e[i];

        WorkingCopyGraph::Edge wu = _instance.edge(w, u);
        WorkingCopyGraph::Edge xu = _instance.edge(x, u);

        //icf(wx)
        if(_instance.getWeight(wu)>0 && _instance.getWeight(xu) >0) {
          icf = icf - min(_instance.getWeight(wu), _instance.getWeight(xu));
        }

        //icp(wx)
        if((_instance.getWeight(wu)<=0 && _instance.getWeight(xu)>0) || ((_instance.getWeight(wu)>0 && _instance.getWeight(xu)<=0))) {
          icp = icp - min(abs(_instance.getWeight(wu)), abs(_instance.getWeight(xu)));
        }
      }

      //s1(u'w)
      double r = _instance.getWeight(_instance.edge(e[0], w)) + _instance.getWeight(_instance.edge(e[1], w));

      //s1(u'x)
      double s = _instance.getWeight(_instance.edge(e[0], x)) + _instance.getWeight(_instance.edge(e[1], x));

      //icf(wx)
      if(s > 0 && r > 0) {
        icf = icf + min(r, s);
      }

      //icp(wx)
      if((s <=0 && r > 0) || (s > 0 && r <= 0)) {
        icp = icp + min(abs(r), abs(s));
      }

      updateICP(wx, icp);
      updateICF(wx, icf);
    }
  }
}

void ParameterizedInstance::afterSetPermanent(const WorkingCopyGraph::Edge &uv, WorkingCopyGraph::Node &proxy) {

  WorkingCopyGraph::Node del = _instance.getGraph().oppositeNode(proxy, uv);

  for(WorkingCopyGraph::NodeIt w(_instance.getGraph()); w != INVALID; ++w) {
    if(w != del && _instance.getGraph().id(w)<_instance.getGraph().id(del)) {
      if(_icp[w]->state(del) == MaxHeap::IN_HEAP) {
        _icp[w]->erase(del);
      }
      if(_icf[w]->state(del) == MaxHeap::IN_HEAP) {
        _icf[w]->erase(del);
      }
    }
    if(w != proxy && w != del) {
      WorkingCopyGraph::Edge pw = _instance.edge(proxy, w);

      //remove edges that became permanent/forbidden through merge of edge proxy -- w
      if(_instance.isPermanent(pw) || _instance.isForbidden(pw)) {
        removeICP(pw);
        removeICF(pw);
      } else {
        double icp = 0;
        double icf = 0;
        computeInducedCosts(pw, icp, icf);

        updateICP(pw, icp);
        updateICF(pw, icf);
      }


    }
  }
}

void ParameterizedInstance::computeLowerBound() {

  double lbValue = 0.0;
  WorkingCopyGraph::EdgeMap<int> crossRef(_instance.getGraph(), -1);
  BinHeap<double, WorkingCopyGraph::EdgeMap<int>, std::greater<double> > edgesOrderByWeight(crossRef);

  if (verbosity > 1)
    cout<< endl<<"computing (edge disjoint) conflict triples... "<<endl;

  int noEdges = countEdges(_instance.getGraph());
  int step = ceil((double) noEdges / 100.0);

  if (verbosity > 1)
    cout<<"\r0%"<<flush;

  for(WorkingCopyGraph::EdgeIt e(_instance.getGraph()); e != INVALID; ++e) {
    edgesOrderByWeight.push(e, abs(_instance.getWeight(e)));
  }

  WorkingCopyGraph::EdgeMap<bool> forbidden(_instance.getGraph(), false);

  WorkingCopyGraph::Edge uv = INVALID;

  int size = edgesOrderByWeight.size();
  for(int i = 0; i < size; ++i) {
    uv = edgesOrderByWeight.top();
    edgesOrderByWeight.pop();


    if (verbosity > 1) {
      if(((i+1) % step) == 0)
        cout<<"\r"<<(int) ((i / (double) noEdges)*100.0) <<"%"<<flush;
    }

    if(forbidden[uv]) {
      continue;
    }

    WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
    WorkingCopyGraph::Node v = _instance.getGraph().v(uv);

    WorkingCopyGraph::Node w = INVALID;
    WorkingCopyGraph::Edge uw = INVALID;
    WorkingCopyGraph::Edge vw = INVALID;
    double maxCost = -numeric_limits<double>::infinity();
    for(WorkingCopyGraph::NodeIt node(_instance.getGraph()); node != INVALID; ++node) {
      if(node == u || node == v) {
        continue;
      }

      WorkingCopyGraph::Edge e1 = _instance.edge(u, node);
      WorkingCopyGraph::Edge e2 = _instance.edge(v, node);

      if(forbidden[e1] || forbidden[e2]) {
        continue;
      }

      if(_instance.getWeight(uv) > 0) {
        if((_instance.getWeight(e1) > 0 && _instance.getWeight(e2) <= 0) || (_instance.getWeight(e1) <= 0 && _instance.getWeight(e2) > 0)) {
          //conflict triple found
          double cost = min(min(_instance.getWeight(uv), abs(_instance.getWeight(e1))), abs(_instance.getWeight(e2)));
          if(cost > maxCost) {
            maxCost = cost;
            w = node;
            uw = e1;
            vw = e2;
          }
        }
      } else {
        if(_instance.getWeight(e1) > 0 && _instance.getWeight(e2) > 0) {
          //conflict triple found
          double cost = min(min(-_instance.getWeight(uv), _instance.getWeight(e1)), _instance.getWeight(e2));
          if(cost > maxCost) {
            maxCost = cost;
            w = node;
            uw = e1;
            vw = e2;
          }
        }
      }
    }

    if(w == INVALID) {
      continue;
    }


    forbidden[uv] = true;
    forbidden[uw] = true;
    forbidden[vw] = true;
    WorkingCopyGraph::Node* uvw = new WorkingCopyGraph::Node[3];
    uvw[0] =u;
    uvw[1] =v;
    uvw[2] =w;

    lbValue += maxCost;
    _nodesLowerBound[u] += maxCost;
    _nodesLowerBound[v] += maxCost;
    _nodesLowerBound[w] += maxCost;


    _edgesLowerBound[uv] = maxCost;
    _edgesLowerBound[uw] = maxCost;
    _edgesLowerBound[vw] = maxCost;

    _conflictTriple[uv] = uvw;
    _conflictTriple[uw] = uvw;
    _conflictTriple[vw] = uvw;

    _conflictTriples.push_back(uvw);
  }

  if (verbosity > 1) {
    cout <<"\rdone."<<endl;
    cout<<"lower bound:\t"<<lbValue<<endl;
    cout<<"-------------------------"<<endl;
  }

  _lbValue = lbValue;
}

double ParameterizedInstance::getLowerBound() {
  return _lbValue;
}

} // namespace ysk
