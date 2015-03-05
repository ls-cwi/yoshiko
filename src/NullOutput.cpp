//
//  NullOutput.cpp
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 21.03.13.
//  Copyright (c) 2013 Emanuel Laude. All rights reserved.
//

#include "NullOutput.h"

using namespace std;

namespace ysk {

void NullOutput::openStream(size_t solution) {
  //
}

void NullOutput::closeStream() {
  //
}

void NullOutput::writeHeader(string label, size_t solution, size_t numberOfNodes, size_t numberOfClusters) {
  //
}

void NullOutput::writeBeginNodes(size_t numberOfNodes) {
  //
}

void NullOutput::writeNode(int nodeId, string name, size_t cluster) {
  //
}

void NullOutput::writeEndNodes() {
  //
}

void NullOutput::writeBeginEdges() {
  //
}

void NullOutput::writeEdge(int sourceId, int targetId, string name, double weight, bool modified) {
  //
}

void NullOutput::writeEndEdges() {
  //
}

void NullOutput::writeBeginCluster(size_t cluster) {
  //
}

void NullOutput::writeEndCluster() {
  //
}

void NullOutput::writeFooter() {
  //
}

} // namespace ysk