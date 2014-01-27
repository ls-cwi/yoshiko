/*
 * File:   HeavyEdgeRule.cpp
 * Author: emanuellaude
 *
 * Created on 27. August 2012, 14:24
 */

#include "HeavyEdgeRule3in1.h"

using namespace std;
using namespace lemon;

HeavyEdgeRule3in1::~HeavyEdgeRule3in1() {
    for(WorkingCopyGraph::NodeIt u(_instance.getGraph()); u != INVALID; ++u) {
        delete _r1[u];
        delete _r2[u];
    }
}

void HeavyEdgeRule3in1::apply() {
    init();
    if (verbosity > 1)
        cout <<"applying rule..."<<flush;
    
    while(_permanent.size() > 0) {
        _success = true;
        WorkingCopyGraph::Edge uv = *_permanent.begin();
        setPermanent(uv);
    }
    
    while(_forbidden.size() > 0) {
        _success = true;
        WorkingCopyGraph::Edge uv = *_forbidden.begin();
        setForbidden(uv);
    }
}

void HeavyEdgeRule3in1::setPermanent(WorkingCopyGraph::Edge& uv) {
    beforeSetPermanent(uv);
    WorkingCopyGraph::Node proxy = _instance.setPermanent(uv);
    afterSetPermanent(uv, proxy);
}

void HeavyEdgeRule3in1::beforeSetPermanent(WorkingCopyGraph::Edge uv) {
    this->_permanent = list<WorkingCopyGraph::Edge>();
    this->_forbidden = list<WorkingCopyGraph::Edge>();
    
    WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
    WorkingCopyGraph::Node v = _instance.getGraph().v(uv);
    

    for(WorkingCopyGraph::NodeIt a(_instance.getGraph()); a != INVALID; ++a) {
        if(a == u || a == v)
            continue;
        
        
        WorkingCopyGraph::NodeIt b(_instance.getGraph(), a);
        ++b;
        for(; b != INVALID; ++b) {
            if(b == u || b == v)
                continue;
            
            WorkingCopyGraph::Edge ab = _instance.edge(a, b);
            if(_instance.isForbidden(ab) || _instance.isPermanent(ab))
                continue;
            
            WorkingCopyGraph::Node e[2];
            e[0] = a;
            e[1] = b;
            
            
            WorkingCopyGraph::Edge xy = ab;

            for(int i = 0; i < 2; i++) {
                WorkingCopyGraph::Node x = e[i];
                WorkingCopyGraph::Node y = e[(i+1)%2];
                

                WorkingCopyGraph::Edge ux = _instance.edge(u, x);
                WorkingCopyGraph::Edge vx = _instance.edge(v, x);
                
                (*_r1[y])[x] = (*_r1[y])[x] + max(0.0, _instance.getWeight(ux)) + max(0.0, _instance.getWeight(vx)) - max(0.0, _instance.getWeight(ux)+ _instance.getWeight(vx));
                (*_r2[y])[x] = (*_r2[y])[x] + abs(_instance.getWeight(ux)) + abs(_instance.getWeight(vx)) - abs(_instance.getWeight(ux) + _instance.getWeight(vx));
                _r3[xy] = _r3[xy] + max(0.0, _instance.getWeight(ux)) + max(0.0, _instance.getWeight(vx)) - max(0.0, _instance.getWeight(ux)+ _instance.getWeight(vx));
            }
            
            if((((*_r1[b])[a] >= 0 || (*_r1[a])[b]>= 0) && !_conserveMultipleSolutions) || (((*_r1[b])[a] > 0 || (*_r1[a])[b]> 0) && _conserveMultipleSolutions)) {
                _forbidden.push_back(ab);
            } else if((((*_r2[b])[a] >= 0|| (*_r2[a])[b]>= 0 || _r3[ab]>= 0) && !_conserveMultipleSolutions) || (((*_r2[b])[a] > 0|| (*_r2[a])[b]> 0 || _r3[ab]> 0) && _conserveMultipleSolutions)) {
                _permanent.push_back(ab);
            }
        }
    }
}

void HeavyEdgeRule3in1::afterSetPermanent(const WorkingCopyGraph::Edge &uv, WorkingCopyGraph::Node &proxy) {
    WorkingCopyGraph::Node h = _instance.getGraph().oppositeNode(proxy, uv);
    
    //TODO is pointer _r1[h] reachable? (h is hidden already)
    delete _r1[h];
    delete _r2[h];
    
    for(WorkingCopyGraph::NodeIt x(_instance.getGraph()); x != INVALID; ++x) {
        if(x == proxy || x == h)
            continue;
        
        WorkingCopyGraph::Edge e = _instance.edge(proxy, x);
        
        if(_instance.isForbidden(e) || _instance.isPermanent(e))
            continue;
        
        computeCosts(e);
        
        if((((*_r1[proxy])[x] >= 0 || (*_r1[x])[proxy]>= 0) && !_conserveMultipleSolutions) || (((*_r1[proxy])[x] > 0 || (*_r1[x])[proxy]> 0) && _conserveMultipleSolutions)) {
            _forbidden.push_back(e);
        } else if((((*_r2[proxy])[x] >= 0|| (*_r2[x])[proxy]>= 0 || _r3[e]>= 0) && !_conserveMultipleSolutions) || (((*_r2[proxy])[x] > 0|| (*_r2[x])[proxy]> 0 || _r3[e]> 0) && _conserveMultipleSolutions)) {
            _permanent.push_back(e);
        }
    }
    
    
}

void HeavyEdgeRule3in1::setForbidden(WorkingCopyGraph::Edge &uv) {
    beforeSetForbidden(uv);
    _instance.setForbidden(uv);
}

void HeavyEdgeRule3in1::beforeSetForbidden(WorkingCopyGraph::Edge &uv) {
    _forbidden.remove(uv);
    WorkingCopyGraph::Node e[2];
    
    e[0] = _instance.getGraph().u(uv);
    e[1] = _instance.getGraph().v(uv);
    
    
    for(WorkingCopyGraph::NodeIt x(_instance.getGraph()); x != INVALID; ++x) {
        if(x == e[0] || x == e[1])
            continue;
        
        for(int i = 0; i < 2; i++) {
            WorkingCopyGraph::Node u = e[i];
            
            WorkingCopyGraph::Edge ux = _instance.edge(u, x);
            
            (*_r2[x])[u] = -numeric_limits<double>::infinity();
            
            if((((*_r2[x])[u] < 0 && (*_r2[u])[x]< 0 && _r3[ux]<0) && !_conserveMultipleSolutions) || (((*_r2[x])[u] <= 0 && (*_r2[u])[x]<= 0 && _r3[ux]<=0) && _conserveMultipleSolutions)) {
                _permanent.remove(ux);
            }
        }
    }
}

void HeavyEdgeRule3in1::init() {
    if (verbosity > 1)
        cout <<endl<<"initializing costs..."<<endl;
    
    for(WorkingCopyGraph::NodeIt u(_instance.getGraph()); u != INVALID; ++u) {
        _r1[u] = new WorkingCopyGraph::NodeMap<double>(_instance.getGraph(), -numeric_limits<double>::infinity());
        _r2[u] = new WorkingCopyGraph::NodeMap<double>(_instance.getGraph(), -numeric_limits<double>::infinity());
    }
    
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
        
        if(_instance.isForbidden(uv) || _instance.isPermanent(uv))
            continue;
        
        computeCosts(uv);
        
        WorkingCopyGraph::Node u = _instance.getGraph().u(uv);
        WorkingCopyGraph::Node v = _instance.getGraph().v(uv);
        
        if((((*_r1[u])[v]>=0 || (*_r1[v])[u]>=0) && !_conserveMultipleSolutions) || (((*_r1[u])[v]>0 || (*_r1[v])[u]>0) && _conserveMultipleSolutions)) {
            _forbidden.push_back(uv);
        } else if((((*_r2[u])[v] >= 0 || (*_r2[v])[u] >= 0 || _r3[uv] >= 0) && !_conserveMultipleSolutions) || (((*_r2[u])[v] > 0 || (*_r2[v])[u] > 0 || _r3[uv] > 0) && _conserveMultipleSolutions)) {
            _permanent.push_back(uv);
        }
    }
    if (verbosity > 1) {
        cout <<"\rdone."<<endl;
    }
}

void HeavyEdgeRule3in1::computeCosts(WorkingCopyGraph::Edge& uv) {
    WorkingCopyGraph::Node e[2];
    
    e[0] = _instance.getGraph().u(uv);
    e[1] = _instance.getGraph().v(uv);
    
    double r3 = 0;
    
    for(int i = 0; i < 2; i++) {
        WorkingCopyGraph::Node u = e[i];
        WorkingCopyGraph::Node v = e[(i+1)%2];
        
        double r1 = 0;
        double r2 = 0;
        
        for (WorkingCopyGraph::IncEdgeIt xv(_instance.getGraph(), v); xv != INVALID; ++xv) {
            if (xv != uv) {
                r1 += max(_instance.getWeight(xv), 0.0);
                r2 += abs(_instance.getWeight(xv));
                r3 += max(_instance.getWeight(xv), 0.0);
            }
        }
        
        (*_r1[u])[v] = -1 * _instance.getWeight(uv) - r1;
        (*_r2[u])[v] = _instance.getWeight(uv) - r2;
    }
    _r3[uv] = _instance.getWeight(uv) - r3;
}

