/*
 * File:   ClusterEditingInstance.cpp
 * Author: emanuellaude, guwek
 *
 * Created on 25. August 2012, 14:50
 */


#include "ClusterEditingInstance.h"

using namespace std;
using namespace lemon;

namespace ysk {

ClusterEditingInstance::~ClusterEditingInstance() {
	if (_isInitialized){
		for (vector<vector<int>*>::iterator it = _vectorCleanList.begin(); it != _vectorCleanList.end(); ++it){
			delete *it;
		}
		delete _workingCopyInstance;
	}
}

void ClusterEditingInstance::init(int size) {
  _orig.resize(size); //Creates a full graph of the given size
  //Initializes and creates a matching WorkingCopyInstance for the full graph
  _workingCopyInstance = new WorkingCopyInstance(this);
  _workingCopyInstance->init();
  _isInitialized = true;
}

void ClusterEditingInstance::initNode(FullGraph::Node node, string name, vector<int>& cluster) {
  _nodeNames[node] = name; //Map node to its name
  _clusters[node] = new vector<int>;
  _vectorCleanList.push_back(_clusters[node]);
  _clusters[node]->insert(_clusters[node]->end(), cluster.begin(), cluster.end());
  _workingCopyInstance->initNode(node, name);
}

void ClusterEditingInstance::initEdge(FullGraph::Edge edge, double weight, EdgeType edgeType) {
  if(abs(weight) != 1)
    _unweighted = false;
  //Check if the edge has a real value (default is int)
  if(floor(weight) != weight) {
    _realValued = true;
  }


  if(weight <= 0) {
    if(_initializedCostInsertion) {
      if(_costInsertion != weight) {
        _dualWeighted = false;
      }
    }

    _costInsertion = weight;
    _initializedCostInsertion = true;
  } else {
    if(_initializedCostDeletion) {
      if(_costDeletion != weight) {
        _dualWeighted = false;
      }
    }

    _costDeletion = weight;
    _initializedCostDeletion = true;
  }

  _weight[edge] = weight;
  _workingCopyInstance->initEdge(edge, weight, edgeType);

  //Set infinite weight to forbidden/permanent edges
  if(edgeType == FORBIDDEN) {
    _forbidden[edge] = true;
    _weight[edge] = -std::numeric_limits<double>::infinity();
  }

  else if(edgeType == PERMANENT) { //forbidden and permanent are mutually exclusive
    _permanent[edge]=true;
    _weight[edge]=std::numeric_limits<double>::infinity();
  }
}

double ClusterEditingInstance::computeCost() const {
  double k = 0;
  const WorkingCopyGraph::NodeMap<vector<int>*>& clusters = _workingCopyInstance->getClusters();
  for(WorkingCopyGraph::NodeIt u(_workingCopyInstance->getGraph()); u != INVALID; ++u) {
    //internal
    for(std::vector<int>::size_type i = 0; i < clusters[u]->size(); i++) {
      FullGraph::Node x = _orig.nodeFromId((*clusters[u])[i]);
      for(std::vector<int>::size_type j = i+1; j < clusters[u]->size(); j++) {
        FullGraph::Node y = _orig.nodeFromId((*clusters[u])[j]);
        double weight = _weight[_orig.edge(x, y)];
        if(weight < 0) {
          k += abs(weight);
        }
      }
    }

    //external
    WorkingCopyGraph::NodeIt v(_workingCopyInstance->getGraph(), u);
    ++v;
    for(; v != INVALID; ++v) {
      double weightWorkingCopyInstance = _workingCopyInstance->getWeight(_workingCopyInstance->edge(u, v));
      for(std::vector<int>::size_type i = 0; i < clusters[u]->size(); i++) {
        FullGraph::Node x = _orig.nodeFromId((*clusters[u])[i]);
        for(std::vector<int>::size_type j = 0; j < clusters[v]->size(); j++) {
          FullGraph::Node y = _orig.nodeFromId((*clusters[v])[j]);
          double weightOrig = _weight[_orig.edge(x, y)];

          if((weightOrig <= 0 && weightWorkingCopyInstance > 0) || (weightOrig > 0 && weightWorkingCopyInstance <= 0)) {
            k += abs(weightOrig);
          }
        }
      }
    }
  }
  return k;

}

//_________________SETTER & GETTER ___________________________//

vector<int>* ClusterEditingInstance::getCluster(FullGraph::Node u) const {
  return _clusters[u];
}

WorkingCopyInstance& ClusterEditingInstance::getWorkingCopyInstance() {
  return *_workingCopyInstance;
}

double ClusterEditingInstance::getWeight(const FullGraph::Edge& e) const {
  return _weight[e];
}

string ClusterEditingInstance::getEdgeName(const FullGraph::Edge e) const {
  stringstream st;
  st << _nodeNames[_orig.u(e)] << " -- " << _nodeNames[_orig.v(e)];
  return st.str();
}

string ClusterEditingInstance::getNodeName(const FullGraph::Node node) const {
  return _nodeNames[node];
}

bool ClusterEditingInstance::isPermanent(const FullGraph::Edge &e) const {
  return _permanent[e];
}

bool ClusterEditingInstance::isForbidden(const FullGraph::Edge &e) const {
  return _forbidden[e];
}

bool ClusterEditingInstance::isDirty() const {
  return _workingCopyInstance->isDirty();
}

int ClusterEditingInstance::getSize() const{
	return _orig.maxNodeId()+1;
}

const FullGraph& ClusterEditingInstance::getOrig() const {
  return _orig;
}


bool ClusterEditingInstance::isDualWeighted() const {
  return _dualWeighted;
}

ostream& operator <<(ostream& o, ClusterEditingInstance &inst) {
  o << inst.getWorkingCopyInstance();
  return o;
}

} // namespace ysk
