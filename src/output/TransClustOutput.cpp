/*
 * TransClustOutput.cpp
 *
 *  Created on: Sep 17, 2017
 *      Author: philipp
 */

#include "TransClustOutput.h"

using namespace std;

namespace ysk {


void TransClustOutput::writeHeader(string label, size_t solution, size_t numberOfNodes, size_t numberOfClusters) {
	//TODO: Write Threshold in here
	_os << "0.5 - ";
}

void TransClustOutput::writeBeginNodes(size_t numberOfNodes) {
    //
}

void TransClustOutput::writeNode(int nodeId, string name, size_t cluster, bool isLast) {
    _os <<name;
	if (!isLast){
		_os << ",";
	}
}

void TransClustOutput::writeEndNodes() {
    //
}

void TransClustOutput::writeBeginEdges() {
    //
}

void TransClustOutput::writeEdge(int sourceId, int targetId, string name, double weight, bool modified) {
    //
}

void TransClustOutput::writeEndEdges() {
    //
}

void TransClustOutput::writeBeginCluster(size_t cluster) {
    //
}

void TransClustOutput::writeEndCluster(bool isLast) {
	if(!isLast){
		_os << ";";
	}
}

void TransClustOutput::writeFooter() {
    //
}

} /* namespace ysk */