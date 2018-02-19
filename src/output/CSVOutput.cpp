#include "../output/CSVOutput.h"

using namespace std;

namespace ysk {

void CSVOutput::writeHeader(string label, size_t solution, size_t numberOfNodes, size_t numberOfClusters) {
  //
}

void CSVOutput::writeBeginNodes(size_t numberOfNodes) {
  //
}

void CSVOutput::writeNode(int nodeId, string name, size_t cluster, bool isLast) {
  _os <<name<<"\t";
}

void CSVOutput::writeEndNodes() {
  //
}

void CSVOutput::writeBeginEdges() {
  //
}

void CSVOutput::writeEdge(int sourceId, int targetId, string name, double weight, bool modified) {
  //
}

void CSVOutput::writeEndEdges() {
  //
}

void CSVOutput::writeBeginCluster(size_t cluster) {
  //
}

void CSVOutput::writeEndCluster(bool isLast) {
  _os<<endl;
}

void CSVOutput::writeFooter() {
  //
}

} // namespace ysk
