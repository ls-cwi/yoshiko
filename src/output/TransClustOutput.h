#ifndef SRC_OUTPUT_TRANSCLUSTOUTPUT_H_
#define SRC_OUTPUT_TRANSCLUSTOUTPUT_H_

#include <iostream>
#include <string>


#include "ClusterEditingOutput.h"


namespace ysk {

/**
 * The output format used by TransClust. This is primarily used for comparison in the ClustEval framework.
 * TODO: Note: The threshold parameter is not implemented in the output file (as irrelevant for ClustEval) and defaults to 0.5
 */
class TransClustOutput : public ClusterEditingOutput {
public:
  TransClustOutput(ClusterEditingInstance& inst,
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

  void writeNode(int nodeId, std::string name, size_t cluster, bool isLast);

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

} /* namespace ysk */

#endif /* SRC_OUTPUT_TRANSCLUSTOUTPUT_H_ */
