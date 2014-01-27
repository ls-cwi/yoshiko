//
//  Delta.h
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 24.01.13.
//  Copyright (c) 2013 Emanuel Laude. All rights reserved.
//

#ifndef DELTA_H
#define DELTA_H

#include <iostream>

#include "WorkingCopyInstance.h"


class Delta {
public:
    Delta() {
        
    }
    
    void setDeltaU(WorkingCopyGraph::Node u, double deltaU);
    void setDeltaV(WorkingCopyGraph::Node v, double deltaV);
    
    double getValue(const WorkingCopyGraph::Node& node);
    
private:
    WorkingCopyGraph::Node _u;
    WorkingCopyGraph::Node _v;
    double _deltaU;
    double _deltaV;
};

#endif /* DELTA_H */
