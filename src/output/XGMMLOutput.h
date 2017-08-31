//
//  XGMMLOutput.h
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 20.03.13.
//  Copyright (c) 2013 Emanuel Laude. All rights reserved.
//

#ifndef XGMMLOUTPUT_H
#define XGMMLOUTPUT_H

#include <iostream>
#include <string>

#include "../output/ClusterEditingOutput.h"

namespace ysk {
  
class XGMMLOutput : public ClusterEditingOutput {
public:
  XGMMLOutput(ClusterEditingInstance& inst,
              ClusterEditingSolutions& solutions,
              std::string filename,
              std::string suffix,
              std::string label)
    : ClusterEditingOutput(inst, solutions, filename, suffix, label)
  {
  }
  
  void writeHeader(std::string label,
                   size_t solution,
                   size_t numberOfNodes,
                   size_t numberOfClusters);
  
  void writeBeginNodes(size_t numberOfNodes);
  
  void writeNode(int nodeId,
                 std::string name,
                 size_t cluster);
  
  void writeEndNodes();
  
  void writeBeginEdges();
  
  void writeEdge(int sourceId,
                 int targetId,
                 std::string name,
                 double weight,
                 bool modified);
  
  void writeEndEdges();
  
  void writeBeginCluster(size_t cluster);
  
  void writeEndCluster();
  
  void writeFooter();
};
  
} // namespace ysk

#endif /* XGMMLOUTPUT_H */
