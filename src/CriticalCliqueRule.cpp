/*
 * File:   CriticalCliqueRule.cpp
 * Author: emanuellaude
 *
 * Created on 26. August 2012, 12:08
 */

#include "CriticalCliqueRule.h"


using namespace std;
using namespace lemon;

void CriticalCliqueRule::apply() {

    for (WorkingCopyGraph::NodeIt v(_instance.getGraph()); v != INVALID; ++v) {
        vector<int>* neighborhoodV = new vector<int>();
        
        //for each node in original instance compute its closed neighborhood
        computeClosedNeighborhood(neighborhoodV, v);
        
        //check if there already exists a node in temp with same neighborhood
        //flag nodeExists set to false if no node with same neighborhood was found
        bool nodeExists = false;
        for (ListGraph::NodeIt w(_temp); w != INVALID; ++w) {
            sort(_neighborhood[w]->begin(), _neighborhood[w]->end());
            sort(neighborhoodV->begin(), neighborhoodV->end());
            if (*_neighborhood[w] == *neighborhoodV) {
                //found a node w with same neighborhood => append node v to w's critical clique
                _criticalClique[w]->push_back(v);
                nodeExists = true;
                break;
            }
            
        }
        
        //no node with same neighborhood found => create new node w in temp with critical clique that only contains v
        if (!nodeExists) {
            ListGraph::Node w = _temp.addNode();
            _criticalClique[w] = new vector<WorkingCopyGraph::Node>();
            _criticalClique[w]->push_back(v);
            _neighborhood[w] = neighborhoodV;
        }
    }
    
    mergeCriticalCliques();
}

//merge critical cliques and deallocate memory
void CriticalCliqueRule::mergeCriticalCliques() {
    for (ListGraph::NodeIt w(_temp); w != INVALID; ++w) {
        if(_criticalClique[w]->size() >= 2) {
            _instance.merge(*_criticalClique[w]);
            _success = true;
        }
    }
}

void CriticalCliqueRule::computeClosedNeighborhood(vector<int>* neighborhood, WorkingCopyGraph::Node& u) {
    for (WorkingCopyGraph::IncEdgeIt e(_instance.getGraph(), u); e != INVALID; ++e) {
        double weight = _instance.getWeight(e);
        if (weight > 0)
            neighborhood->push_back(_instance.getGraph().id(_instance.getGraph().oppositeNode(u, e)));
    }
    neighborhood->push_back(_instance.getGraph().id(u));
}

CriticalCliqueRule::~CriticalCliqueRule() {
    for (ListGraph::NodeIt w(_temp); w != INVALID; ++w) {
        delete _criticalClique[w];
        delete _neighborhood[w];
    }
}
