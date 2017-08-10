/*
 * LibraryInput.h
 *
 *  Created on: Aug 9, 2017
 *      Author: Philipp Spohr
 */

#ifndef SRC_INPUT_LIBRARYINPUT_H_
#define SRC_INPUT_LIBRARYINPUT_H_

#include "ClusterEditingInput.h"

namespace yskInput{

	/**
	 * Simple helper struct that contains relevant information about edges in the most rudimentary fashion.
	 */
	struct edge{
		int sourceID;
		int targetID;
		double cost;
	};

	class LibraryInput : public ClusterEditingInput{

		public:

			ysk::ClusterEditingInstance* parseInput();

			LibraryInput(){
				_size = 0;
				_edges = std::list<edge>();
			};

			/**
			 * Sets |V| of the problem instance
			 * @param id The required size of the problem
			 */
			void setSize(int id);

			/**
			 * Adds an edge between the given node ids (or updates the cost).
			 * Note: The program uses a full graph internally so all edges exist.
			 * @param sourceID The id of the source node
			 * @param targetID The id of the target node
			 * @param cost The new cost that is to be assigned
			 */
			void addEdge(int sourceID,int targetID,double cost);


		private:
			/**
			 * |V| of the problem instance
			 */
			int _size;
			/**
			 * A simple list holding edges
			 */
			std::list<edge> _edges;

	};

}


#endif /* SRC_INPUT_JENAINPUT_H_ */
