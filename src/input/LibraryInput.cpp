/*
 * LibraryInput.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: philipp
 */

#include "LibraryInput.h"

#include <stdexcept>

using namespace lemon;
using namespace std;
using namespace ysk;

namespace yskInput{
		
	void LibraryInput::parseInput(){
		_instance->init(_size);
		return;
	};
	

	void LibraryInput::addEdge(int sourceID,int targetID,double cost){
		//Out of range edges
		if (sourceID >= _size || targetID >= _size){
			throw std::invalid_argument("Source or Target ID of the node are out of range (Did you initialize the input with the correct size?");
		}
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
