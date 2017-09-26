#include "RowSimInput.h"


using namespace lemon;
using namespace std;
using namespace ysk;

namespace yskInput {

	//without shame stolen and modified from SIFInput.cpp
	bool RowSimInput::parseInput(std::istream &is){
		if (verbosity > 4){
			cout << "Parsing RowSim input ..." << endl << std::flush;
		}
		try {
			string line;

			//Fetch maximum to normalize
			double max = -DBL_MAX;

			while (getline(is, line)) {
				vector<string> tokens;
				tokenize(line, tokens, " \t\"\r");
				double similarity = atof(tokens[2].c_str());
				if (similarity < 0.0){
					cerr << "Found invalid similarity value of: " << tokens[2] <<", terminating parsing process!" <<endl;
					return false;
				}
				if (similarity > max){
					max = similarity;
				}
			}

			if (verbosity > 3){
				cout << "Maximum in RowSimInput: " << max << endl;
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

			_instance->init(n);

			FullGraph fullGraph = _instance ->getOrig();

			for (FullGraph::NodeIt v(fullGraph); v != INVALID; ++v) {
				vector<int> cluster;
				cluster.push_back(fullGraph.id(v));
				_instance->initNode(v, nodeName[fullGraph.id(v)], cluster);
			}

			// initialize edges to be absent
			for (FullGraph::EdgeIt e(fullGraph); e != INVALID; ++e)
				_instance->initEdge(e, 0, UNDECIDED);

			//jump back to beginning of stream
			is.clear();
			is.seekg(0, ios::beg);

			while (getline(is, line)) {
				vector<string> tokens;
				tokenize(line, tokens, " \t\"\r");
				if (tokens[0] == tokens[1]) cerr << "skipping self loop " << tokens[0] << " -- " << tokens[1] << endl;
				else
				{
					double similarity = atof(tokens[2].c_str());
					double edgeWeight = (similarity/max) - _threshold;
					FullGraph::Edge e = fullGraph.edge(fullGraph.nodeFromId(nodeID[tokens[0]]), fullGraph.nodeFromId(nodeID[tokens[1]]));
					//cout << "Generating edge from " << nodeID[tokens[0]] << " to " << nodeID[tokens[1]] << " with weight: " << edgeWeight << endl;
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
