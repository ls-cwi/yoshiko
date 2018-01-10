/*
 *  clusterEditingSolutions.cpp
 *
 *
 *  Created by Gunnar W. Klau on 30-11-2012.
 *  Copyright 2011 Centrum Wiskunde & Informatica (CWI). All rights reserved.
 *
 */

#include "ClusterEditingSolutions.h"


using namespace std;
using namespace lemon;

namespace ysk {

	size_t ClusterEditingSolutions::getNumberOfClusters(size_t i) const {
	  return _solutions[i].size();
	}

	vector<int>& ClusterEditingSolutions::getCluster(size_t i, size_t k) {
	  return _solutions[i][k];
	}

	vector<vector<int> >& ClusterEditingSolutions::getSolution(size_t i) {
		//TODO: Throw exception if the index is invalid?
		return _solutions[i];
	}

	void ClusterEditingSolutions::resize(long numberOfSolutions) {
	  _solutions.resize(numberOfSolutions);
	}

	size_t ClusterEditingSolutions::getNumberOfSolutions() const {
	  return _solutions.size();
	}

	void ClusterEditingSolutions::setSolution(int k, const ClusterEditingInstance &inst) {
	  _solutions[k].resize(countNodes(inst.getOrig()));
	  int i = 0;
	  for(FullGraph::NodeIt u(inst.getOrig()); u != INVALID; ++u, i++) {
		_solutions[k][i].push_back(inst.getOrig().id(u));
	  }

	}

	void ClusterEditingSolutions::setSolution(int k, const WorkingCopyInstance& inst) {
	  _solutions[k].resize(countNodes(inst.getGraph()));

	  int i = 0;
	  for(WorkingCopyGraph::NodeIt u(inst.getGraph()); u != INVALID; ++u, i++) {
		for(vector<int>::iterator it = inst.getClusters()[u]->begin(); it != inst.getClusters()[u]->end(); ++it) {
		  _solutions[k][i].push_back(*it);
		}
	  }
	}

	void ClusterEditingSolutions::setSolution(int k, const IloNumArray &x_vals, const ClusterEditingInstance &i) {
	  // compute clusters
	  if (verbosity > 1)
		cout << "computing clusters for solution " << k << "... " << flush;

	  const FullGraph g = i.getOrig();

	  ListGraph c; // graph that will contain clusters as (fully) connected components

	  FullGraph::NodeMap<ListGraph::Node> A(g);

	  for (FullGraph::NodeIt v(g); v != INVALID; ++v)
		A[v] = c.addNode();

	  for (FullGraph::EdgeIt e(g); e != INVALID; ++e)
		if (x_vals[g.id(e)] > 1 - eps)
		  c.addEdge(A[g.u(e)], A[g.v(e)]);

	  ListGraph::NodeMap<int> comp_num(c);
	  _solutions[k].resize(connectedComponents(c, comp_num));

	  for (FullGraph::NodeIt v(g); v != INVALID; ++v)
		_solutions[k][comp_num[A[v]]].push_back(g.id(v));

	  if (verbosity > 1)
		cout << "done." << endl;
	}

	SolutionFlags ClusterEditingSolutions::getFlags(){
		return _flags;
	}

	void ClusterEditingSolutions::setFlags(SolutionFlags f){
		_flags = f;
	}

	void ClusterEditingSolutions::printSolution(size_t index){
		for(auto &entry : _solutions[index]){
			cout << "Cluster: ";
			for (auto i = entry.begin(); i != entry.end(); ++i)
			    cout << *i<<" ";
			cout <<endl;
		}
		cout << endl;
	}


} // namespace ysk
