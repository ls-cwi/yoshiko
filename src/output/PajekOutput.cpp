//
//  PajekOutput.cpp
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 26.03.13.
//  Copyright (c) 2013 Emanuel Laude. All rights reserved.
//

#include "../output/PajekOutput.h"

using namespace std;

namespace ysk {

void PajekOutput::writeHeader(string label, size_t solution, size_t numberOfNodes, size_t numberOfClusters) {
  //
}

void PajekOutput::writeBeginNodes(size_t numberOfNodes) {
  _os <<"*Vertices "<<numberOfNodes<<endl;
}

void PajekOutput::writeNode(int nodeId, string name, size_t cluster, bool isLast) {
  _os << "\t"<<nodeId<<"\t\""<<name<<"\""<<"\t"<<cluster<<endl;
}

void PajekOutput::writeEndNodes() {
  //
}

void PajekOutput::writeBeginEdges() {
  _os <<"*Arcs"<<endl;
  _os <<"*Edges"<<endl;
}

void PajekOutput::writeEdge(int sourceId, int targetId, string name, double weight, bool modified) {
  _os<<"\t"<<sourceId<<"\t"<<targetId<<"\t"<<weight;

  if(modified) {
    _os<<"\t"<<1<<endl;
  } else {
    _os<<"\t"<<0<<endl;
  }
}

void PajekOutput::writeEndEdges() {
  //
}

void PajekOutput::writeBeginCluster(size_t cluster) {
  //
}

void PajekOutput::writeEndCluster(bool isLast) {
  //
}

void PajekOutput::writeFooter() {
  //
}

} // namespace ysk
