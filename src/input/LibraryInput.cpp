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
		
	ysk::ClusterEditingInstance* LibraryInput::parseInput(std::istream &is){
		return new ClusterEditingInstance();
	};
	
	bool LibraryInput::validateInput(std::istream &is){
		//TODO
		return true;
	}
	

	void LibraryInput::addEdge(int sourceID,int targetID,double cost){
		edge tmp;
		tmp.sourceID = sourceID;
		tmp.targetID = targetID;
		tmp.cost = cost;
		_edges.push_back(tmp);
	};
	
	void LibraryInput::setSize(int id){
		
	};
	


}
