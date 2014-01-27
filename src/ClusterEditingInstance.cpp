/*
 * File:   ClusterEditingInstance.cpp
 * Author: emanuellaude
 *
 * Created on 25. August 2012, 14:50
 */


#include "ClusterEditingInstance.h"

using namespace std;
using namespace lemon;

ClusterEditingInstance::~ClusterEditingInstance() {
    for(FullGraph::NodeIt u(_orig); u != INVALID; ++u) {
        delete _clusters[u];
    }
    delete _workingCopyInstance;
}

void ClusterEditingInstance::init(int size) {
    _orig.resize(size);
    _workingCopyInstance = new WorkingCopyInstance(this);
    _workingCopyInstance->init();
}

void ClusterEditingInstance::initNode(FullGraph::Node node, string name, vector<int>& cluster) {
    _nodeNames[node] = name;
    _clusters[node] = new vector<int>;
    _clusters[node]->insert(_clusters[node]->end(), cluster.begin(), cluster.end());
    _workingCopyInstance->initNode(node, name);
}

void ClusterEditingInstance::initEdge(FullGraph::Edge edge, double weight, bool permanent, bool forbidden) {
    if(abs(weight) != 1)
        _unweighted = false;
    
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
    _workingCopyInstance->initEdge(edge, weight, permanent, forbidden);
    
    if(forbidden) {
        _forbidden[edge] = true;
        _weight[edge] = -std::numeric_limits<double>::infinity();
    }
    
    if(permanent) {
        _permanent[edge]=true;
        _weight[edge]=std::numeric_limits<double>::infinity();
    }
}

void ClusterEditingInstance::parseJenaFormat(std::istream &is) {
	try {
		string line;
        
		getline(is, line); // first line contains number of nodes
		int n = atoi(line.c_str());
        
		init(n);
        
		for (FullGraph::NodeIt v(_orig); v != INVALID; ++v) {
            getline(is, line);
            vector<int> cluster;
            cluster.push_back(_orig.id(v));
            initNode(v, line, cluster);
		}
        
		int i = 0;
		for (FullGraph::NodeIt v(_orig); i < n - 1; ++v, ++i) {
			getline(is, line);
			vector<string> tokens;
			tokenize(line, tokens, " \t\"");
			int j = 0;
			FullGraph::NodeIt w(_orig, v);
			++w;
			for (; w != INVALID; ++w) {
                FullGraph::Edge e = _orig.edge(v, w);
                
                double weight = 0.0;
                bool permanent = false;
                bool forbidden = false;
                
                if (tokens[j] == "inf") {
					permanent = true;
                    weight = std::numeric_limits<double>::infinity();
				}
                else if (tokens[j] == "-inf") {
					forbidden = true;
                    weight = -std::numeric_limits<double>::infinity();
				} else {
                    weight = atof(tokens[j].c_str());
                    if (weight >= 1e+20) {
                        permanent = true;
                        weight = std::numeric_limits<double>::infinity();
                    } else if (weight <= -1e+20) {
                        forbidden = true;
                        weight = -std::numeric_limits<double>::infinity();
                    }
                }
                initEdge(e, weight, permanent, forbidden);
                ++j;
			}
		}
	} catch (Exception &e) {
		cerr << "caught exception while parsing the graph" << endl;
		cerr << e.what() << endl;
		exit(-1);
	}
}

void ClusterEditingInstance::parseCleverFormat(std::istream &is) {
	try {
		string line;
		getline(is, line);
		vector<string> tokens;
		tokenize(line, tokens, " \t\"");
		int n = (int) tokens.size() + 1;
		init(n);
        
		for (FullGraph::NodeIt v(_orig); v != INVALID; ++v) {
            vector<int> cluster;
            cluster.push_back(_orig.id(v));
            stringstream buffer;
            buffer<<_orig.id(v);
            initNode(v, buffer.str(), cluster);
		}
        
		//jump back to beginning of stream
		is.clear();
		is.seekg(0, ios::beg);
        
		for (int i = 0; i < n - 1; ++i) {
			getline(is, line);
			vector<string> tokens;
			tokenize(line, tokens, " \t\"");
			for (int j = i + 1; j < n; ++j) {
				FullGraph::Edge e = _orig.edge(_orig(i), _orig(j));
                
                double weight = 0.0;
                bool permanent = false;
                bool forbidden = false;
                
                if (tokens[j - i - 1] == "inf") {
					permanent = true;
                    weight = std::numeric_limits<double>::infinity();
				} else if (tokens[j - i - 1] == "-inf") {
					forbidden = true;
                    weight = -std::numeric_limits<double>::infinity();
				} else {
                    weight = atof(tokens[j - i - 1].c_str());
                }
                
                initEdge(e, weight, permanent, forbidden);
			}
		}
        
	} catch (Exception &e) {
		cerr << "caught exception while parsing the graph" << endl;
		cerr << e.what() << endl;
		exit(-1);
	}
}


void ClusterEditingInstance::parseSIFFormat(std::istream &is) {
	try {
        map<string, int> nodeID;
        map<int, string> nodeName;
        string line; int n = 0;
        while (getline(is, line)) {
            vector<string> tokens;
            tokenize(line, tokens, " \t\"");
            if (nodeID.find(tokens[0]) == nodeID.end()) {
                nodeID[tokens[0]] = n; nodeName[n++] = tokens[0];
            }
            
            if (nodeID.find(tokens[2]) == nodeID.end()) {
                nodeID[tokens[2]] = n; nodeName[n++] = tokens[2];
            }
        }
        
        //cout << "parsed " << n << " nodes" << endl;
        init(n);
        
		for (FullGraph::NodeIt v(_orig); v != INVALID; ++v) {
            vector<int> cluster;
            cluster.push_back(_orig.id(v));
            initNode(v, nodeName[_orig.id(v)], cluster);
		}
        
        // initialize edges to be absent
        for (FullGraph::EdgeIt e(_orig); e != INVALID; ++e)
            initEdge(e, -1.0, false, false);
        
        //jump back to beginning of stream
		is.clear();
		is.seekg(0, ios::beg);
        
        while (getline(is, line)) {
            vector<string> tokens;
            tokenize(line, tokens, " \t\"");
            FullGraph::Edge e = _orig.edge(_orig.nodeFromId(nodeID[tokens[0]]), _orig.nodeFromId(nodeID[tokens[2]]));
            initEdge(e, 1.0, false, false);
        }
        
        
	} catch (Exception &e) {
		cerr << "caught exception while parsing the graph" << endl;
		cerr << e.what() << endl;
		exit(-1);
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

const FullGraph& ClusterEditingInstance::getOrig() const {
    return _orig;
}

bool ClusterEditingInstance::isUnweighted() const {
    return _unweighted;
}

bool ClusterEditingInstance::isDualWeighted() const {
    return _dualWeighted;
}

bool ClusterEditingInstance::isRealValued() const {
    return _realValued;
}

ostream& operator <<(ostream& o, ClusterEditingInstance &inst) {
    o << inst.getWorkingCopyInstance();
    return o;
}
