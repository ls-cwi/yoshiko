#include "SolutionChecker.h"

using namespace ysk;
using namespace std;
using namespace lemon;

bool SolutionChecker::verifySolutionIsCliqueGraph(ClusterEditingInstance & instance,  ClusterEditingSolutions & solutions){
for (unsigned int i = 0; i < solutions.getNumberOfSolutions(); i++){
        
        cout << "Verifying that solution "<< i <<" is a valid solution for WCE ..." << endl;
        
        vector<vector<int>> solution = solutions.getSolution(i);
        
        set<FullGraph::Edge> existingEdges;
        
        
        //Check which edges exist in the proposed solution
        for (unsigned int c = 0; c < solution.size(); c++){
            vector<int> cluster = solution[c];
            //cout << "Cluster" << c <<endl;

            for (unsigned int m = 0; m < cluster.size()-1; m++) {
                for (unsigned int n = m+1; n < cluster.size(); n++) {
                    //cout << m << n << endl;
                    FullGraph::Node node1 = instance.getOrig().nodeFromId(cluster[m]);
                    FullGraph::Node node2 = instance.getOrig().nodeFromId(cluster[n]);
                    FullGraph::Edge edge = instance.getOrig().findEdge(node1,node2);
                    existingEdges.insert(edge);
                }
            }
        }
        
        //int correctAmountOfTriangles = (instance.getSize()*(instance.getSize()-1)*(instance.getSize()-2))/6;
        //int countTriangles = 0;
        //TODO: fancy flush function to display progress?
        cout << "Expecting " << correctAmountOfTriangles << " triangles ...";
        //Check for conflict triples, intentionally not using the Lemon Node iterator, more variance is good for testing :)
        for (unsigned int x = 0; x < instance.getSize(); x++){
            for (unsigned int y = x+1; y < instance.getSize(); y++){
                for (unsigned int z = y+1; z < instance.getSize(); z++){
                    //countTriangles ++;
                    //cout << x<< y << z <<  "  " << countTriangles <<endl;
                    
                    int edgeCount = 0;
                    FullGraph::Node nodeX = instance.getOrig().nodeFromId(x);
                    FullGraph::Node nodeY = instance.getOrig().nodeFromId(y);
                    FullGraph::Node nodeZ = instance.getOrig().nodeFromId(z);
                    
                    FullGraph::Edge edge1 = instance.getOrig().findEdge(nodeX,nodeY);
                    FullGraph::Edge edge2 = instance.getOrig().findEdge(nodeY,nodeZ);
                    FullGraph::Edge edge3 = instance.getOrig().findEdge(nodeX,nodeZ);
                    
                    if (existingEdges.find(edge1) != existingEdges.end()){ edgeCount++;}
                    if (existingEdges.find(edge2) != existingEdges.end()){ edgeCount++;}
                    if (existingEdges.find(edge3) != existingEdges.end()){ edgeCount++;}

                    if (edgeCount == 2) {
                        cout << "CONFLICT TRIPLE FOUND!!!" << endl;
                        return false;
                    }
                }
            }  
        }

    }
    cout << "Solution validity verified!" << endl;
    return true;
}

//TODO: args are const
//TODO: Excessive running time, can be greatly reduced if the software simply tracks which edges are added and which are not, on the other hand not relying on previously calculated stuff adds a bit of redundancy and security
bool SolutionChecker::verifySolutionCosts( ClusterEditingInstance & instance,  ClusterEditingSolutions & solutions){
    for (unsigned int i = 0; i < solutions.getNumberOfSolutions(); i++){
        
        cout << "Verifying the editing costs of solution " << i << " ..." << endl;
        
        vector<vector<int>> solution = solutions.getSolution(i);
        
        double calculatedCosts = 0.0;
        set<FullGraph::Edge> existingEdges;
        
        
        //Check which edges exist in the proposed solution
        for (unsigned int c = 0; c < solution.size(); c++){
            vector<int> cluster = solution[c];
            //cout << "Cluster" << c <<endl;

            for (unsigned int m = 0; m < cluster.size()-1; m++) {
                for (unsigned int n = m+1; n < cluster.size(); n++) {
                    //cout << m << n << endl;
                    FullGraph::Node node1 = instance.getOrig().nodeFromId(cluster[m]);
                    FullGraph::Node node2 = instance.getOrig().nodeFromId(cluster[n]);
                    FullGraph::Edge edge = instance.getOrig().findEdge(node1,node2);
                    existingEdges.insert(edge);
                }
            }
        }

        
        for (FullGraph::EdgeIt edge(instance.getOrig());edge != INVALID; ++edge){
            if (existingEdges.find(edge) != existingEdges.end()){
                //Existing edges with negative weight add to our editing costs
                if(instance.getWeight(edge) < 0){
                    calculatedCosts -= instance.getWeight(edge);
                }
            }
            else{
                //Non-existing edges with positive weight add to our editing costs
                if(instance.getWeight(edge) > 0){
                    calculatedCosts += instance.getWeight(edge);
                }
            }
            //cout << calculatedCosts << " after processing edge " << instance.getEdgeName(edge) << endl;
        }

        if (solutions.getFlags().totalCost != calculatedCosts){
                cout << "Could not verify the editing costs ... found :" << solutions.getFlags().totalCost << " while it should be " << calculatedCosts << endl;
                return false;
        }
    }
    cout << "Solution costs verified!" << endl;
    return true;
}

