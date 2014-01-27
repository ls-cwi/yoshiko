/*
 * File:   WorkingCopyInstance.cpp
 * Author: emanuellaude
 *
 * Created on 28. August 2012, 16:54
 */

#include "WorkingCopyInstance.h"

using namespace std;
using namespace lemon;

WorkingCopyInstance::~WorkingCopyInstance() {
    
    for(WorkingCopyGraph::NodeIt v(*_graph); v != INVALID; ++v) {
        delete (*_clusters)[v];
    }

    delete _nodeNames;
    delete _weight;
    delete _permanent;
    delete _forbidden;
    delete _clusters;
    
    delete _graph;
    delete _edges;
    delete _nodes;
}

void WorkingCopyInstance::init() {
    _nodes = new FullGraph::NodeMap<bool>(_instance->getOrig());
    _edges = new FullGraph::EdgeMap<bool>(_instance->getOrig());
    _graph = new WorkingCopyGraph(_instance->getOrig(), *_nodes, *_edges);
    
    _nodeNames = new WorkingCopyGraph::NodeMap<string>(*_graph);
    _weight = new WorkingCopyGraph::EdgeMap<double>(*_graph);
    _permanent = new WorkingCopyGraph::EdgeMap<bool>(*_graph, false);
    _forbidden = new WorkingCopyGraph::EdgeMap<bool>(*_graph, false);
    _clusters = new WorkingCopyGraph::NodeMap<vector<int>* >(*_graph);
}

void WorkingCopyInstance::initNode(FullGraph::Node node, string name) {
    (*_nodes)[node] = true;
    (*_nodeNames)[node] = name;
    (*_clusters)[node] = new vector<int>;
    (*_clusters)[node]->insert((*_clusters)[node]->end(), _graph->id(node));
}

void WorkingCopyInstance::initEdge(FullGraph::Edge edge, double weight, bool permanent, bool forbidden) {
    (*_edges)[edge] = true;
    (*_weight)[edge] = weight;
    
    if(forbidden) {
        (*_forbidden)[edge] = true;
        (*_weight)[edge] = -std::numeric_limits<double>::infinity();
    }
    
    if(permanent) {
        (*_permanent)[edge]=true;
        (*_weight)[edge]=std::numeric_limits<double>::infinity();
    }
}

//first node in vector nodes serves as proxy.
WorkingCopyGraph::Node WorkingCopyInstance::merge(const vector<WorkingCopyGraph::Node>& nodes) {
    if(nodes.size() == 0) {
        return INVALID;
    }
    
    //choose one node as a proxy to represent the cluster
    WorkingCopyGraph::Node proxy = nodes[0];
    
    if(nodes.size() == 1) {
        return proxy;
    }
    
    _dirty = true;
    
    //compute new weights
    for (WorkingCopyGraph::NodeIt u(*_graph); u != INVALID; ++u) {
        if(find(nodes.begin(), nodes.end(), u) != nodes.end()) {
            continue;
        }
        
        double weight = 0;
        for (vector<WorkingCopyGraph::Node>::const_iterator v = nodes.begin(); v != nodes.end(); v++) {
            
            weight += (*_weight)[edge(u, *v)];
        }
        (*_weight)[edge(u, proxy)] = weight;
        
        if(weight == numeric_limits<double>::infinity()) {
            (*_permanent)[edge(u, proxy)] = true;
        }
        if(weight == -numeric_limits<double>::infinity()) {
            (*_forbidden)[edge(u, proxy)] = true;
        }
    }
    
    
    //merge nodes and compute new node name
    stringstream buffer;
    buffer << (*_nodeNames)[proxy];
    
    for (vector<WorkingCopyGraph::Node>::const_iterator u = nodes.begin(); u != nodes.end(); u++) {
        if ((*_nodes)[*u] == false) {
            cerr << "error: trying to merge invisible nodes!" << endl;
            exit(-1);
        }
        
        if(*u == proxy)
            continue;
        
        
        (*_clusters)[proxy]->insert((*_clusters)[proxy]->end(), (*_clusters)[*u]->begin(), (*_clusters)[*u]->end());
        delete (*_clusters)[*u];
        _instance->getCluster(proxy)->insert(_instance->getCluster(proxy)->end(), _instance->getCluster(*u)->begin(), _instance->getCluster(*u)->end());
        
        buffer << "/" << (*_nodeNames)[*u];
        (*_nodes)[*u] = false;
    }
    
    (*_nodeNames)[proxy] = buffer.str();
    
    return proxy;
}

WorkingCopyGraph::Node WorkingCopyInstance::setPermanent(const WorkingCopyGraph::Edge &uv) {
    WorkingCopyGraph::Node u = _instance->getOrig().u(uv);
    WorkingCopyGraph::Node v = _instance->getOrig().v(uv);
    
    if (!(*_nodes)[u] || !(*_nodes)[v] || !(*_edges)[uv]) {
        cerr << "Fatal error: trying to modify invisible edge "<<_instance->getOrig().id(u)<<" -- "<<_instance->getOrig().id(v) << endl;
        cerr.flush();
        
        exit(-1);
    }
    
    setPermanent(uv, u);
    return u;
}

void WorkingCopyInstance::setPermanent(const WorkingCopyGraph::Edge &uv, const WorkingCopyGraph::Node& proxy) {
    WorkingCopyGraph::Node u = _instance->getOrig().u(uv);
    WorkingCopyGraph::Node v = _instance->getOrig().v(uv);
    
    if (!(*_nodes)[u] || !(*_nodes)[v] || !(*_edges)[uv]) {
        cerr << "Fatal error: trying to modify invisible edge "<<_instance->getOrig().id(u)<<" -- "<<_instance->getOrig().id(v) << endl;
        cerr.flush();
        
        exit(-1);
    }
    
    if((*_permanent)[uv] || (*_forbidden)[uv]) {
        cerr << "Fatal error: trying to modify permanent/forbidden edge "<<_graph->id(_graph->u(uv))<<" -- "<<_graph->id(_graph->v(uv)) << endl;
        cerr.flush();
        
        exit(-1);
    }
    if(proxy != u && proxy != v) {
        cerr << "Fatal error: trying to set edge "<<_graph->id(_graph->u(uv))<<" -- "<<_graph->id(_graph->v(uv)) <<" to permanent but proxy "<< _graph->id(proxy)<< " is not an end node of edge!"<< endl;
        cerr.flush();
        
        exit(-1);
    }
    
    vector<WorkingCopyGraph::Node> nodes;
    nodes.resize(2);
    nodes[0] = proxy;
    nodes[1] = _graph->oppositeNode(proxy, uv);
    merge(nodes);
}

WorkingCopyGraph::Edge WorkingCopyInstance::edge(const WorkingCopyGraph::Node& u, const WorkingCopyGraph::Node& v) const {
    if(u == v) {
        cerr << "Fatal error: loop edges are not defined" <<endl;
        cerr.flush();
        
        exit(-1);
    }
    WorkingCopyGraph::Edge uv = _instance->getOrig().edge(u, v);
    if (!(*_nodes)[u] || !(*_nodes)[v] || !(*_edges)[uv]) {
        cerr << "Fatal error: trying to acces invisible edge "<<_instance->getOrig().id(u)<<" -- "<<_instance->getOrig().id(v) << endl;
        cerr.flush();
        
        exit(-1);
    }
    
    return uv;
}

double WorkingCopyInstance::getWeight(const WorkingCopyGraph::Edge& e) const {
    return (*_weight)[e];
}

const WorkingCopyGraph::EdgeMap<double>& WorkingCopyInstance::getWeight() const {
    return *_weight;
}

const WorkingCopyGraph::NodeMap<string>& WorkingCopyInstance::getNodeNames() const {
    return *_nodeNames;
}

string WorkingCopyInstance::getNodeName(const WorkingCopyGraph::Node &u) const {
    return (*_nodeNames)[u];
}

string WorkingCopyInstance::getEdgeName(const WorkingCopyGraph::Edge &uv) const {
	stringstream st;
	st << (*_nodeNames)[_graph->u(uv)] << " -- " << (*_nodeNames)[_graph->v(uv)];
	return st.str();
}

bool WorkingCopyInstance::isPermanent(const WorkingCopyGraph::Edge &e) const {
    return (*_permanent)[e];
}

bool WorkingCopyInstance::isForbidden(const WorkingCopyGraph::Edge &e) const {
    return (*_forbidden)[e];
}

bool WorkingCopyInstance::isDirty() const {
    return _dirty;
}

const WorkingCopyGraph::EdgeMap<bool>& WorkingCopyInstance::getPermanent() const {
    return *_permanent;
}

const WorkingCopyGraph::EdgeMap<bool>& WorkingCopyInstance::getForbidden() const {
    return *_forbidden;
}

const WorkingCopyGraph::NodeMap<vector<int>*>& WorkingCopyInstance::getClusters() const {
    return *_clusters;
}

void WorkingCopyInstance::setForbidden(const WorkingCopyGraph::Edge& uv) {
    WorkingCopyGraph::Node u = _instance->getOrig().u(uv);
    WorkingCopyGraph::Node v = _instance->getOrig().v(uv);
    
    if (!(*_nodes)[u] || !(*_nodes)[v] || !(*_edges)[uv]) {
        cerr << "Fatal error: trying to modify invisible edge "<<_instance->getOrig().id(u)<<" -- "<<_instance->getOrig().id(v) << endl;
        cerr.flush();
        
        exit(-1);
    }
    
    if((*_permanent)[uv] || (*_forbidden)[uv]) {
        cerr << "Fatal error: trying to modify permanent/forbidden edge "<<_graph->id(_graph->u(uv))<<" -- "<<_graph->id(_graph->v(uv)) << endl;
        cerr.flush();
        
        exit(-1);
    }
    
    (*_forbidden)[uv] = true;
    _dirty = true;
    (*_weight)[uv] = -std::numeric_limits<double>::infinity();
}

const WorkingCopyGraph& WorkingCopyInstance::getGraph() const {
    return *_graph;
}

const FullGraph& WorkingCopyInstance::getOrig() const {
    return _instance->getOrig();
}

ostream& operator <<(ostream& o, WorkingCopyInstance &inst) {
    graphWriter(inst.getGraph(), o).nodeMap("nodeNames", inst.getNodeNames())
    .edgeMap("weight", inst.getWeight()).edgeMap("forbidden", inst.getForbidden())
    .edgeMap("permanent", inst.getPermanent()).attribute("caption", "WORKINGCOPYINSTANCE").run();
    
    return o;
}

