//
//  GMLOutput.cpp
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 21.03.13.
//  Copyright (c) 2013 Emanuel Laude. All rights reserved.
//

#include "../output/GMLOutput.h"

using namespace std;

namespace ysk {

void GMLOutput::writeHeader(string label,
                            size_t solution,
                            size_t numberOfNodes,
                            size_t numberOfClusters) {
  _os<<"graph ["<<endl;
  _os<<"\tcomment \""<<label<<"\""<<endl;
  _os<<"\tdirected 0"<<endl;
  _os<<"\tid "<<solution<<endl;
  _os<<"\tlabel \""<<label<< "_"<<solution<<"\""<<endl;
}

void GMLOutput::writeBeginNodes(size_t numberOfNodes) {
  //
}

void GMLOutput::writeNode(int nodeId, string name, size_t cluster) {
  _os<<"\tnode ["<<endl;
  _os<<"\t\tid "<<nodeId<<endl;
  _os<<"\t\tlabel \""<<name<<"\""<<endl;
  _os<<"\t\tcluster "<<cluster<<endl;
  _os<<"\t]"<<endl;
}

void GMLOutput::writeEndNodes() {
  //
}

void GMLOutput::writeBeginEdges() {
  //
}

void GMLOutput::writeEdge(int sourceId,
                          int targetId,
                          string name,
                          double weight,
                          bool modified) {
  _os<<"\tedge ["<<endl;
  _os<<"\t\tsource "<<sourceId<<endl;
  _os<<"\t\ttarget "<<targetId<<endl;
  _os<<"\t\tlabel \""<<name<<"\""<<endl;
  _os<<"\t\tweight \""<<weight<<"\""<<endl;
  
  if(modified) {
    _os<<"\t\tmodified "<<1<<endl;
  } else {
    _os<<"\t\tmodified "<<0<<endl;
  }
  
  _os<<"\t]"<<endl;
}

void GMLOutput::writeEndEdges() {
  //
}

void GMLOutput::writeBeginCluster(size_t cluster) {
  //
}

void GMLOutput::writeEndCluster() {
  //
}

void GMLOutput::writeFooter() {
  _os<<"]"<<endl;
}
  
} // namespace ysk
