#include "SIFInput.h"

using namespace lemon;
using namespace std;
using namespace ysk;


namespace yskInput{

bool SIFInput::parseInput(std::istream &is){
	//TWO PASS

	try {
		map<string, int> nodeID;
		map<int, string> nodeName;

		string line; int n = 0;

		while (getline(is, line)) {
			vector<string> tokens;
			// gunnar: there was a bug in reading in SIF files (DOS line break). I added '\r' so it should work now
			tokenize(line, tokens, " \t\"\r");
			if (nodeID.find(tokens[0]) == nodeID.end()) {
				nodeID[tokens[0]] = n; nodeName[n++] = tokens[0];
			}

			if (nodeID.find(tokens[2]) == nodeID.end()) {
				nodeID[tokens[2]] = n; nodeName[n++] = tokens[2];

			}
		}
		if (verbosity > 4){
			cout << "[SIF Parser]: Read " << n << " nodes" << endl;
		}

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
			_instance->initEdge(e, -1.0, UNDECIDED);
		//cout << "done." << endl;

		//jump back to beginning of stream
		is.clear();
		is.seekg(0, ios::beg);

		while (getline(is, line)) {
			vector<string> tokens;
			// gunnar: same here, added 'r'
			tokenize(line, tokens, " \t\"\r");
			if (tokens[0] == tokens[2]) cerr << "skipping self loop " << tokens[0] << " -- " << tokens[2] << endl;
			else
			{
				FullGraph::Edge e = fullGraph.edge(fullGraph.nodeFromId(nodeID[tokens[0]]), fullGraph.nodeFromId(nodeID[tokens[2]]));
				_instance->initEdge(e, 1.0, UNDECIDED);
			}
		}
	} catch (Exception &e) {
		cerr << "caught exception while parsing the graph" << endl;
		cerr << e.what() << endl;
		return false;
	}
	return true;
}
}
