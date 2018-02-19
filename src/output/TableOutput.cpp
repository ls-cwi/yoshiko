#include "../output/TableOutput.h"

using namespace std;

namespace ysk {

void TableOutput::writeHeader(string label, size_t solution, size_t numberOfNodes, size_t numberOfClusters) {
    _os<<numberOfNodes<<endl;
    _os<<numberOfClusters<<endl;
    _os<<"name\tcluster"<<endl;
}

void TableOutput::writeBeginNodes(size_t numberOfNodes) {
    //
}

void TableOutput::writeNode(int nodeId, string name, size_t cluster, bool isLast) {
    _os <<name<<"\t"<<cluster<<endl;
}

void TableOutput::writeEndNodes() {
    //
}

void TableOutput::writeBeginEdges() {
    //
}

void TableOutput::writeEdge(int sourceId, int targetId, string name, double weight, bool modified) {
    //
}

void TableOutput::writeEndEdges() {
    //
}

void TableOutput::writeBeginCluster(size_t cluster) {
    //
}

void TableOutput::writeEndCluster(bool isLast) {
    //
}

void TableOutput::writeFooter() {
    //
}

} // namespace ysk
