//
//  ClusterEditingOutput.h
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 20.03.13.
//  Copyright (c) 2013 Emanuel Laude. All rights reserved.
//

#ifndef CLUSTEREDITINGOUTPUT_H
#define CLUSTEREDITINGOUTPUT_H

#include <iostream>
#include <string>

#include "Globals.h"
#include "ClusterEditingInstance.h"
#include "ClusterEditingSolutions.h"
#include "WorkingCopyInstance.h"

class ClusterEditingOutput {
public:
  ClusterEditingOutput(ClusterEditingInstance& inst, ClusterEditingSolutions& solutions, std::string filename, std::string suffix, std::string label) : _label(label), _instance(inst), _solutions(solutions), _filename(filename), _suffix(suffix) {
    }
    
  static ClusterEditingOutput* newInstance(ClusterEditingInstance& inst, ClusterEditingSolutions& solutions, std::string filename, std::string label, int format);
    
    void write();
    virtual void openStream(size_t solution);
    virtual void closeStream();
    
  virtual void writeHeader(std::string label, size_t solution, size_t numberOfNodes, size_t numberOfClusters) = 0;
    virtual void writeBeginNodes(size_t numberOfNodes) = 0;
  virtual void writeNode(int nodeId, std::string name, size_t cluster) = 0;
    virtual void writeEndNodes() = 0;
    virtual void writeBeginEdges() = 0;
  virtual void writeEdge(int sourceId, int targetId, std::string name, double weight, bool modified) = 0;
    virtual void writeEndEdges() = 0;
    virtual void writeBeginCluster(size_t cluster) = 0;
    virtual void writeEndCluster() = 0;
    virtual void writeFooter() = 0;
    
    
    virtual ~ClusterEditingOutput();
private:
  std::string _label;
    ClusterEditingInstance& _instance;
    ClusterEditingSolutions& _solutions;
    
protected:
  std::string _filename;
  std::string _suffix;
  std::ofstream _os;
};

#endif /* CLUSTEREDITINGOUTPUT_H */
