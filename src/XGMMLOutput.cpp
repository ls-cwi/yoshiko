//
//  XGMMLOutput.cpp
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 20.03.13.
//  Copyright (c) 2013 Emanuel Laude. All rights reserved.
//

#include "XGMMLOutput.h"

using namespace std;

namespace ysk {
  
void XGMMLOutput::writeHeader(string label, size_t solution, size_t numberOfNodes, size_t numberOfClusters) {
    _os<<"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"<<endl;
    _os<<"<graph label=\""<<label<< "_"<<solution<<"\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\""<<endl;
    _os<<"\t\txmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\""<<endl;
    _os<<"\t\txmlns:cy=\"http://www.cytoscape.org\" xmlns=\"http://www.cs.rpi.edu/XGMML\" directed=\"0\">"<<endl;
}

void XGMMLOutput::writeBeginNodes(size_t numberOfNodes) {
    //
}

void XGMMLOutput::writeNode(int nodeId, string name, size_t cluster) {
    _os<<"\t<node label=\""<<name<<"\" id=\""<<nodeId<<"\">"<<endl;
    _os<<"\t\t<att type=\"string\" name=\"canonicalName\" value=\""<<name<<"\" />"<<endl;
    _os<<"\t\t<att type=\"integer\" name=\"cluster\" value=\""<<cluster<<"\" />"<<endl;
    _os<<"\t\t<graphics type=\"ELLIPSE\" h=\"40.0\" w=\"40.0\" fill=\"#ccccff\" width=\"1\" outline=\"#b0b0b0\" cy:nodeLabel=\""<<name<<"\" cy:borderLineType=\"solid\"/>"<<endl;
    _os<<"\t</node>"<<endl;
}

void XGMMLOutput::writeEndNodes() {
    //
}

void XGMMLOutput::writeBeginEdges() {
    //
}

void XGMMLOutput::writeEdge(int sourceId, int targetId, string name, double weight, bool modified) {
    _os<<"\t<edge label=\""<<name<<"\" source=\""<<sourceId<<"\" target=\""<<targetId<<"\">"<<endl;
    _os<<"\t\t<att name=\"weight\" type=\"integer\" value=\""<<weight<<"\" />"<<endl;
    if(weight <= 0 && modified) {
        //inserted
        _os<<"\t\t<att name=\"modified\" type=\"boolean\" value=\"true\" />"<<endl;
        _os<<"\t\t<graphics width=\"1\" fill=\"#00f000\" />"<<endl;
        
    } else if(weight > 0 && modified) {
        //deleted
        _os<<"\t\t<att name=\"modified\" type=\"boolean\" value=\"true\" />"<<endl;
        _os<<"\t\t<graphics width=\"1\" fill=\"#ffb5c5\" />"<<endl;
    } else if(!modified) {
        //not modified
        _os<<"\t\t<att name=\"modified\" type=\"boolean\" value=\"false\" />"<<endl;
        _os<<"\t\t<graphics width=\"1\" fill=\"#b0b0b0\" />"<<endl;
    } else {
        //
    }
    
    _os<<"\t</edge>"<<endl;
}

void XGMMLOutput::writeEndEdges() {
    //
}

void XGMMLOutput::writeBeginCluster(size_t cluster) {
    //
}

void XGMMLOutput::writeEndCluster() {
    //
}

void XGMMLOutput::writeFooter() {
    _os<<"</graph>"<<endl;
}

} // namespace ysk
