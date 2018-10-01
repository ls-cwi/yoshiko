#include "JENAInput.h"

using namespace lemon;
using namespace std;
using namespace ysk;

namespace yskInput{



	bool JENAInput::parseInput(std::istream &is){

		try {
			//STEP 1: PARSE NODE COUNT
			string line;

			getline(is, line); // first line contains number of nodes
			int n = atoi(line.c_str());

			_instance->init(n); //init "blank" full graph with n nodes

			//STEP 2: PARSE NODE NAMES

			//iterates over all nodes of the newly created graph
			FullGraph fullGraph = _instance ->getOrig();

			for (FullGraph::NodeIt v(fullGraph); v != INVALID; ++v) {
				getline(is, line);
				vector<int> cluster;
				cluster.push_back(fullGraph.id(v)); //Each node is initialized with its "own cluster"
				_instance ->initNode(v, line, cluster);
			}

			//STEP 3: PARSE EDGES
			int i = 0;
			//"Source"-Node loop
			for (FullGraph::NodeIt v(fullGraph); i < n - 1; ++v, ++i) {
				getline(is, line);
				vector<string> tokens;
				tokenize(line, tokens, " \t\""); //Tokenize using whitespace as seperator

				int j = 0;
				FullGraph::NodeIt w(fullGraph, v);
				++w; //First edge from node 0 -> 0 can be skipped

				//"Target"-Node loop
				for (; w != INVALID; ++w) {
					FullGraph::Edge e = fullGraph.edge(v, w);
					//Default values
					double weight = 0.0;
					EdgeType edgeType = UNDECIDED;

					//Process permanent edges (by conventions those ar marked as infinite in the JENA file format)
					if (tokens[j] == "inf") {
						edgeType = PERMANENT;
						weight = std::numeric_limits<double>::infinity();
					}
					else if (tokens[j] == "-inf") { //Forbidden edges
						edgeType = FORBIDDEN;
						weight = -std::numeric_limits<double>::infinity();
					}
					else { //Weighted edges
						weight = atof(tokens[j].c_str());
						//Out of bounds values are assumed to be infinity
// 						if (weight >= 1e+20) {
// 							edgeType = PERMANENT;
// 							weight = std::numeric_limits<double>::infinity();
// 						}
// 						else if (weight <= -1e+20) {
// 							edgeType = FORBIDDEN;
// 							weight = -std::numeric_limits<double>::infinity();
// 						}
						if (weight >= std::numeric_limits<double>::max()) {
							edgeType = PERMANENT;
							weight = std::numeric_limits<double>::infinity();
						}
						else if (weight <= std::numeric_limits<double>::min()) {
							edgeType = FORBIDDEN;
							weight = -std::numeric_limits<double>::infinity();
						}
					}
					_instance ->initEdge(e, weight, edgeType);
					++j;
				}//TARGET NODE LOOP END
			}//SOURCE NODE LOOP END
		}
		catch (Exception &e) {
			//TODO: MORE INFO ABOUT WHAT WENT WRONG
			cerr << "caught exception while parsing the graph" << endl;
			cerr << e.what() << endl;
			return false;
		}
		return true;
	};

}



