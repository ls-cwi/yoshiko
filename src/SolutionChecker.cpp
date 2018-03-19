#include "SolutionChecker.h"

using namespace ysk;
using namespace std;
using namespace lemon;

bool SolutionChecker::verifySolutionIsCliqueGraph(){
    //TODO:
    return true;
}

//TODO: args are const
//TODO: Excessive running time, can be greatly reduced if the software simply tracks which edges are added and which are not, on the other hand not relying on previously calculated stuff adds a bit of redundancy and security
bool SolutionChecker::verifySolutionCosts( ClusterEditingInstance & instance,  ClusterEditingSolutions & solutions){
    for (unsigned int i = 0; i < solutions.getNumberOfSolutions(); i++){
        
        cout << "Verifying the Solution Costs of Solution " << i << " ..." << endl;
        
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
            cout << calculatedCosts << " after processing edge " << instance.getEdgeName(edge) << endl;
        }

        if (solutions.getFlags().totalCost != calculatedCosts){
                cout << "Could not verify the editing costs ... found :" << solutions.getFlags().totalCost << " while it should be " << calculatedCosts << endl;
                return false;
        }
    }
    cout << "Solution costs verified!" << endl;
    return true;
}

