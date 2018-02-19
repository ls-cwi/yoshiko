#include "MCLOutput.h"


using namespace std;

namespace ysk {

void MCLOutput::writeHeader(string label,
                            size_t solution,
                            size_t numberOfNodes,
                            size_t numberOfClusters) {}

void MCLOutput::writeBeginNodes(size_t numberOfNodes) {
  //
}

void MCLOutput::writeNode(int nodeId, string name, size_t cluster, bool isLast) {
	_os << name << "\t";
}

void MCLOutput::writeEndNodes() {
  //
}

void MCLOutput::writeBeginEdges() {
  //
}

void MCLOutput::writeEdge(int sourceId,
                          int targetId,
                          string name,
                          double weight,
                          bool modified) {}

void MCLOutput::writeEndEdges() {}

void MCLOutput::writeBeginCluster(size_t cluster) {}

void MCLOutput::writeEndCluster(bool isLast) {
		if (!isLast){
			_os << endl;
		}
}

void MCLOutput::writeFooter() {}

} // namespace ysk
