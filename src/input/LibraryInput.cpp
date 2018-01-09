/*
 * LibraryInput.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: philipp
 */

#include "LibraryInput.h"

using namespace lemon;
using namespace std;
using namespace ysk;

namespace yskInput{
		
	bool LibraryInput::parseInput(){


		//We are now adjusting the ClusterEditingInstance accordingly to the previously assigned size and edges
		_instance->init(_size);

		FullGraph fullGraph = _instance ->getOrig(); //Created in the init function

		//NODE initialization

		//In this representation the nodes don't have names and are simply referred to by index
		unsigned long id = 0;
		for (FullGraph::NodeIt v(fullGraph); v != INVALID; ++v, id++) {
			vector<int> cluster;
			cluster.push_back(fullGraph.id(v)); //Each node is initialized with its "own cluster"
			_instance ->initNode(v,"LNODE "+id, cluster); //We simply name each node with its index
		}

		//Edge parsing

		//**
		//DUMMY INIT FOR ALL EDGES
		for (FullGraph::EdgeIt v(fullGraph); v!= INVALID; ++v) {
			_instance ->initEdge(v, _defaultInsertionCost, UNDECIDED);
		}

		//**

		//Loop over edges
		for (std::vector<edge>::iterator it = _edges.begin(); it != _edges.end(); ++it){

			edge currentEdge = *it;

			//cout << "DEBUG: Parsing EDGE: " << currentEdge.sourceID << "--" << currentEdge.targetID << ":";

			//LOOPS ARE NOT RELEVANT FOR CLUSTERING IN OUR MODEL
			if (currentEdge.sourceID == currentEdge.targetID){
				continue;
			}

			FullGraph::Edge e = fullGraph.edge(
						fullGraph.nodeFromId(currentEdge.sourceID),
						fullGraph.nodeFromId(currentEdge.targetID)
					);
			_instance ->initEdge(e, currentEdge.cost, currentEdge.edgeType);

			//cout << "SUCCESS!" << "\n";


		}

		if (verbosity > 2) cout << "Success! LibraryInput parsed!" << endl;
		return true;
	};
	
	void LibraryInput::addEdge(unsigned long sourceID,unsigned long targetID,double cost){

		//cout << "DEBUG: ADDED EDGE " <<  sourceID << "--" << targetID << "(" << cost << ")" << "\n";

		LibraryInput::addEdge(sourceID,targetID,cost,false,false);
	}

	void LibraryInput::addEdge(unsigned long sourceID,unsigned long targetID,double cost,bool permanent, bool forbidden){
		//Out of range edges
		if (sourceID >= _size || targetID >= _size){
			throw std::invalid_argument("Source or Target ID of the node are out of range (Did you initialize the input with the correct size?");
		}
		edge tmp;
		tmp.sourceID = sourceID;
		tmp.targetID = targetID;
		tmp.cost = cost;
		tmp.edgeType = UNDECIDED;
		if (permanent){
			tmp.edgeType = PERMANENT;
		}
		else if (forbidden){
			tmp.edgeType = FORBIDDEN;
		}
		_edges.push_back(tmp);
	};
	
	//SETTER / GETTER

	void LibraryInput::setDefaultInsertionCost(double cost){
		_defaultInsertionCost = cost;
	}

	void LibraryInput::setSize(unsigned long size){
		_size = size;
	};
	
}
