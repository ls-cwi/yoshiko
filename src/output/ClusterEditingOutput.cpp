//
//  ClusterEditingOutput.cpp
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 20.03.13.
//  Copyright (c) 2013 Emanuel Laude. All rights reserved.
//

#include "../output/ClusterEditingOutput.h"

#include "../output/CSVOutput.h"
#include "../output/GMLOutput.h"
#include "../output/NullOutput.h"
#include "../output/PajekOutput.h"
#include "../output/TableAppOutput.h"
#include "../output/TableOutput.h"
#include "../output/XGMMLOutput.h"

using namespace std;
using namespace lemon;

namespace ysk {

ClusterEditingOutput* ClusterEditingOutput::newInstance(ClusterEditingInstance &inst, ClusterEditingSolutions &solutions, string filename, string label, int format) {
  
  if(filename.size() > 0) {
    switch(format) {
      case 0:
        return new CSVOutput(inst, solutions, filename, ".csv", label);
      case 1:
        return new TableOutput(inst, solutions, filename, ".txt", label);
      case 2:
        return new GMLOutput(inst, solutions, filename, ".gml", label);
      case 3:
        return new XGMMLOutput(inst, solutions, filename, ".xgmml", label);
      case 4:
        return new PajekOutput(inst, solutions, filename, ".net", label);
      case 5:
        return new TableAppOutput(inst, solutions, filename, ".txt", label);
      default:
        return new NullOutput(inst, solutions, filename, "", label);
    }
  } else {
    return new NullOutput(inst, solutions, filename, "", label);
  }
}

void ClusterEditingOutput::write() {
  
  if(verbosity > 3) {
    cout << "=========================" << endl;
    cout <<"solutions:"<<endl;
    cout<<"-------------------------"<<endl;
  }
  for(size_t i = 0; i < _solutions.getNumberOfSolutions(); i++) {
    openStream(i);
    
    double insertionCost = 0.0, deletionCost = 0.0;
    int deletionCount = 0, insertionCount = 0;
    
    size_t numberOfNodes = countNodes(_instance.getWorkingCopyInstance().getGraph());
    
    writeHeader(_label, i, numberOfNodes, _solutions.getNumberOfClusters(i));
    writeBeginNodes(numberOfNodes);
    if(verbosity > 3) {
      cout <<endl<< "=========================" << endl;
      cout << "=========================" << endl<<endl;
      cout <<"solution no "<<i+1<<":" <<endl;
      cout << "=========================" << endl;
      cout << "partition:" << endl;
      cout <<"-------------------------" << endl;
    }
    
    for(size_t k = 0; k < _solutions.getNumberOfClusters(i); k++) {
      if(verbosity > 3)
        cout<<"cluster no "<<k+1<<":"<<endl<<"\t";
      
      
      writeBeginCluster(k);
      
      for(vector<int>::iterator it = _solutions.getCluster(i, k).begin(); it != _solutions.getCluster(i, k).end(); it++) {
        FullGraph::Node u = _instance.getOrig().nodeFromId(*it);
        if(verbosity > 3) {
          cout <<_instance.getNodeName(u)<<" ";
        }
        
        writeNode(*it, _instance.getNodeName(u), k);
      }
      
      if(verbosity > 3) {
        cout <<endl;
      }
      
      writeEndCluster();
    }
    writeEndNodes();
    
    writeBeginEdges();
    if (verbosity > 3) {
      cout << "=========================" << endl;
      cout << "modifications:" << endl;
      cout<<"-------------------------"<<endl;
    }
    
    for(size_t k = 0; k < _solutions.getNumberOfClusters(i); k++) {
      //internal
      vector<int>& v = _solutions.getCluster(i, k);
      for(vector<int>::size_type l = 0; l < v.size(); l++) {
        FullGraph::Node x = _instance.getOrig().nodeFromId(v[l]);
        for(std::vector<int>::size_type j = l+1; j < v.size(); j++) {
          FullGraph::Node y = _instance.getOrig().nodeFromId(v[j]);
          
          FullGraph::Edge xy = _instance.getOrig().edge(x, y);
          double weight = _instance.getWeight(xy);
          string name = _instance.getEdgeName(xy);
          int sourceId = v[l];
          int targetId = v[j];
          
          if(weight <= 0) {
            if (verbosity > 3)
              cout << "insertion of edge " << name << " (cost " << abs(weight) << ")" << endl;
            
            insertionCost+= abs(weight);
            insertionCount++;
            
            writeEdge(sourceId, targetId, name, weight, true);
          } else {
            writeEdge(sourceId, targetId, name, weight, false);
          }
        }
      }
      
      //external
      for(vector<vector<int> >::size_type l = k+1; l < _solutions.getSolution(i).size(); l++) {
        vector<int>& w = _solutions.getCluster(i, l);
        
        for(vector<int>::size_type i = 0; i < v.size(); i++) {
          FullGraph::Node x = _instance.getOrig().nodeFromId(v[i]);
          for(std::vector<int>::size_type j = 0; j < w.size(); j++) {
            FullGraph::Node y = _instance.getOrig().nodeFromId(w[j]);
            
            FullGraph::Edge xy = _instance.getOrig().edge(x, y);
            double weight = _instance.getWeight(xy);
            string name = _instance.getEdgeName(xy);
            int sourceId = v[i];
            int targetId = w[j];
            
            if(weight > 0) {
              if (verbosity > 3)
                cout << "deletion of edge " << name << " (cost " << abs(weight) << ")" << endl;
              
              deletionCost+= abs(weight);
              deletionCount++;
              
              writeEdge(sourceId, targetId, name, weight, true);
            }
          }
        }
      }
    }
    
    writeEndEdges();
    
    if (verbosity > 3) {
      cout << "----------------------------------" << endl;
      cout <<"total insertion cost ("<< insertionCount<<" insertions):\t"<<insertionCost<<endl;
      cout <<"total deletion cost ("<<deletionCount<<" deletions):\t"<<deletionCost<<endl;
      cout <<"=================================="<<endl;
      cout <<"total cost ("<<insertionCount+deletionCount<<" modifications):\t"<<insertionCost+deletionCost<<endl;
    }
    
    writeFooter();
    closeStream();
  }
}

void ClusterEditingOutput::openStream(size_t solution) {
  stringstream str;
  str<<_filename<<"_"<<solution<<_suffix;
  _os.open (str.str().c_str());
  
  if(!_os.is_open()) {
    cerr<< "Error: Cannot create/open file: "<<str.str()<<endl;
    exit(-1);
  }
}

void ClusterEditingOutput::closeStream() {
  if(_os.is_open()) {
    _os.close();
  }
}

ClusterEditingOutput::~ClusterEditingOutput() {
}

} // namespace ysk
