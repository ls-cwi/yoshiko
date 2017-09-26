/*
 * MCLOutput.h
 *
 *  Created on: Sep 23, 2017
 *      Author: philipp
 */

#ifndef SRC_OUTPUT_MCLOUTPUT_H_
#define SRC_OUTPUT_MCLOUTPUT_H_

#include <iostream>
#include <string>

#include "../output/ClusterEditingOutput.h"

namespace ysk {

class MCLOutput : public ClusterEditingOutput {
public:
  MCLOutput(ClusterEditingInstance& inst,
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
  void writeEndNodes();
  void writeNode(int nodeId,
                 std::string name,
                 size_t cluster,
				 bool isLast);
  void writeBeginEdges();
  void writeEdge(int sourceId,
                 int targetId,
                 std::string name,
                 double weight,
                 bool modified);
  void writeEndEdges();
  void writeBeginCluster(size_t cluster);
  void writeEndCluster(bool isLast);
  void writeFooter();
};

} // namespace ysk

#endif /* SRC_OUTPUT_MCLOUTPUT_H_ */
