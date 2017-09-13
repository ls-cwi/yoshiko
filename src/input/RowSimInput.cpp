#include "RowSimInput.h"


using namespace lemon;
using namespace std;
using namespace ysk;

namespace yskInput {

	RowSimInput::RowSimInput(ysk::ClusterEditingInstance* inst, double threshold):StreamInput(inst){
		_threshold = threshold;
	}

	//without shame stolen and modified from SIFInput.cpp
	bool RowSimInput::parseInput(std::istream &is){
		try {
			string line;


			//Fetch maximum to normalize
			double max = -DBL_MAX;

			while (getline(is, line)) {
				vector<string> tokens;
				tokenize(line, tokens, " \t\"\r");
				double similarity = stod(tokens[2]);
				if (similarity < 0.0){
					cerr << "Found invalid similarity value of: " << std::to_string(similarity) <<", terminating parsing process!" <<endl;
					return false;
				}
				if (similarity > max){
					max = similarity;
				}
			}

			//jump back to beginning of stream
			is.clear();
			is.seekg(0, ios::beg);

			//Fetch nodes

			map<string, int> nodeID;
			map<int, string> nodeName;
			int n = 0;
			while (getline(is, line)) {
				vector<string> tokens;
				tokenize(line, tokens, " \t\"\r");
				if (nodeID.find(tokens[0]) == nodeID.end()) {
					nodeID[tokens[0]] = n; nodeName[n++] = tokens[0];
				}

				if (nodeID.find(tokens[1]) == nodeID.end()) {
					nodeID[tokens[1]] = n; nodeName[n++] = tokens[1];

				}
			}
			//cout << "read " << n << " nodes" << endl;

			//cout << "initializing... " << flush;
			_instance->init(n);
			//cout << "done." << endl;
			FullGraph fullGraph = _instance ->getOrig();

			//cout << "setting up clusters... " << flush;
			for (FullGraph::NodeIt v(fullGraph); v != INVALID; ++v) {
				vector<int> cluster;
				cluster.push_back(fullGraph.id(v));
				_instance->initNode(v, nodeName[fullGraph.id(v)], cluster);
			}
			//cout << "done." << endl;

			//cout << "initializing edges... " << flush;
			// initialize edges to be absent
			for (FullGraph::EdgeIt e(fullGraph); e != INVALID; ++e)
				_instance->initEdge(e, 0, UNDECIDED);
			//cout << "done." << endl;

			//jump back to beginning of stream
			is.clear();
			is.seekg(0, ios::beg);

			while (getline(is, line)) {
				vector<string> tokens;
				tokenize(line, tokens, " \t\"\r");
				if (tokens[0] == tokens[1]) cerr << "skipping self loop " << tokens[0] << " -- " << tokens[1] << endl;
				else
				{
					double similarity = stod(tokens[2]);
					if (similarity < 0.0 || similarity > 1.0){
						cerr << "Invalid similarity score of: "+std::to_string(similarity) << "terminating!" << endl;
						return false;
					}
					double edgeWeight = (similarity/max) - _threshold;
					FullGraph::Edge e = fullGraph.edge(fullGraph.nodeFromId(nodeID[tokens[0]]), fullGraph.nodeFromId(nodeID[tokens[2]]));
					_instance->initEdge(e, edgeWeight, UNDECIDED);
				}
			}
		} catch (Exception &e) {
			cerr << "caught exception while parsing the graph" << endl;
			cerr << e.what() << endl;
			return false;
		}
		return true;
	}

} /* namespace ysk */
