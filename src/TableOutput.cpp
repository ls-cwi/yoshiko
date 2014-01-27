//
//  TableOutput.cpp
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 21.03.13.
//  Copyright (c) 2013 Emanuel Laude. All rights reserved.
//

#include "TableOutput.h"

using namespace std;


void TableOutput::writeHeader(string label, size_t solution, size_t numberOfNodes, size_t numberOfClusters) {
    _os<<numberOfNodes<<endl;
    _os<<numberOfClusters<<endl;
    _os<<"name\tcluster"<<endl;
}

void TableOutput::writeBeginNodes(size_t numberOfNodes) {
    //
}

void TableOutput::writeNode(int nodeId, string name, size_t cluster) {
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

void TableOutput::writeEndCluster() {
    //
}

void TableOutput::writeFooter() {
    //
}