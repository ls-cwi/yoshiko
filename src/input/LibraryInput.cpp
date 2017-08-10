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
		//CASE 1: There are edges defined with node ids beyond the current size of the instance
		for (std::list<edge>::iterator it = _edges.begin();it!=_edges.end();it++){
			if (it->sourceID >= _size || it->targetID >= _size){
				cout << "Invalid input! One edge declaration contains a node that is outside the scope. Have you initialized the input size correctly?";
				return false;
			}
		}
		
		//All good
		return true;
	}
	

	void LibraryInput::addEdge(int sourceID,int targetID,double cost){
		edge tmp;
		tmp.sourceID = sourceID;
		tmp.targetID = targetID;
		tmp.cost = cost;
		_edges.push_back(tmp);
	};
	
	//SETTER / GETTER
	
	void LibraryInput::setSize(int size){
		_size = size;
	};
	


}
