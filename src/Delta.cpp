//
//  Delta.cpp
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 24.01.13.
//  Copyright (c) 2013 Emanuel Laude. All rights reserved.
//

#include "Delta.h"

using namespace std;

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
