/*
 * LibraryInput.h
 *
 *  Created on: Aug 9, 2017
 *      Author: Philipp Spohr
 */

#ifndef SRC_INPUT_LIBRARYINPUT_H
#define SRC_INPUT_LIBRARYINPUT_H

#include "ClusterEditingInput.h"
#include "ClusterEditingInstance.h"
#include <stdexcept>


namespace yskInput{

	/**
	 * Simple helper struct that contains relevant information about edges in the most rudimentary fashion.
	 */
	struct edge{
		unsigned long sourceID;
		unsigned long targetID;
		double cost;
		ysk::EdgeType edgeType;
	};

	/**
	 * A more abstract input type. This input type represents an "empty" input that can be modified by function calls instead of parsing.
	 * It is thus useful for library calls / external object generation
	 * Note: This basically wraps the more complex class "ClusterEditingInstance", providing an external representation.
	 */
	class LibraryInput : public ClusterEditingInput{

		public:

			/**
			 * Default constructor
			 */
			LibraryInput():ClusterEditingInput(new ysk::ClusterEditingInstance()){
				_edges = std::vector<edge>();
				_size = 0;
				_defaultInsertionCost = -1;
			}

			~LibraryInput(){
				delete _instance;
			}

			/**
			 * Attempts to create a fitting ClusterEditingInstance matching the provided information
			 * @return True if successful, False otherwise
			 */
			bool parseInput();

			/**
			 * Adds an non-permanent non-forbidden edge between the given node ids (or updates the cost).
			 * Note: The program uses a full graph internally so all edges exist.
			 * @param sourceID The id of the source node
			 * @param targetID The id of the target node
			 * @param cost The new cost that is to be assigned
			 */
			void addEdge(unsigned long sourceID,unsigned long targetID,double cost);

			/**
			 * Adds an edge between the given node ids (or updates the cost).
			 * Note: The program uses a full graph internally so all edges exist.
			 * @param sourceID The id of the source node
			 * @param targetID The id of the target node
			 * @param cost The new cost that is to be assigned
			 * @param permanent Marks the edge as permanent (takes precedence over forbidden!)
			 * @param forbidden Marks the edge as forbidden (no effect if edge is marked as permanent!)
			 */
			void addEdge(unsigned long sourceID,unsigned long targetID,double cost,bool permanent, bool forbidden);

			//SETTER / GETTER METHODS

			/**
			 * Sets |V| of the problem instance
			 * @param id The required size of the problem
			 */
			void setSize(unsigned long id);

			void setDefaultInsertionCost(double cost);

		private:
			/**
			 * |V| of the problem instance
			 */
			unsigned long _size;

			double _defaultInsertionCost;
			/**
			 * A simple list holding edges
			 */
			std::vector<edge> _edges;

	};

}


#endif /* SRC_INPUT_JENAINPUT_H */
