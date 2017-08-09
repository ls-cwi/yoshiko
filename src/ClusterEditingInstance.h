/*
 * File:   ClusterEditingInstance.h
 * Author: emanuellaude
 *
 * Created on 25. August 2012, 14:50
 */

#ifndef CLUSTEREDITINGINSTANCE_H
#define	CLUSTEREDITINGINSTANCE_H

#include <lemon/adaptors.h>
#include <lemon/lgf_writer.h>
#include <lemon/full_graph.h>

#include <vector>
#include <cmath>
#include <string>
#include <limits>
#include <map>

#include "WorkingCopyInstance.h"
#include "Helpers.h"

namespace ysk {

/**
 * Compile reference for WCI
 */
class WorkingCopyInstance;

/**
 * Describes an instance of the double-weighted graph clustering problem. Note that this class
 * should not be directly used to modify the graph, instead a working copy should be used.
 */
class ClusterEditingInstance {
public:
  
	/**
	 * Default constructor, initializes an empty instance of the dwgc-problem.
	 */
  ClusterEditingInstance()
    : _unweighted(true)
    , _dualWeighted(true)
    , _realValued(false)
    , _costInsertion(0.0)
    , _costDeletion(0.0)
    , _initializedCostInsertion(false)
    , _initializedCostDeletion(false)
    , _nodeNames(_orig)
    , _weight(_orig)
    , _permanent(_orig)
    , _forbidden(_orig)
    , _clusters(_orig)
  {
  }
  
  virtual ~ClusterEditingInstance();
  
  void init(int size);
  
  void initNode(lemon::FullGraph::Node node,
                std::string name,
                std::vector<int>& cluster);
  
  void initEdge(lemon::FullGraph::Edge edge,
                double weight,
                bool permanent,
                bool forbidden);
  
  WorkingCopyInstance& getWorkingCopyInstance();
  
  const lemon::FullGraph& getOrig() const;
  
  double computeCost() const;
  
  double getWeight(const lemon::FullGraph::Edge& e) const;
  
  bool isUnweighted() const;
  
  bool isDualWeighted() const;
  
  bool isRealValued() const;
  
  std::string getEdgeName(const lemon::FullGraph::Edge e) const;
  
  std::string getNodeName(const lemon::FullGraph::Node node) const;
  
  bool isPermanent(const lemon::FullGraph::Edge& e) const;
  
  bool isForbidden(const lemon::FullGraph::Edge& e) const;
  
  bool isDirty() const;
  
  void parseJenaFormat(std::istream &is);
  
  void parseCleverFormat(std::istream &is);
  
  void parseSIFFormat(std::istream &is);
  
  std::vector<int>* getCluster(lemon::FullGraph::Node u) const;
  
  friend std::ostream& operator <<(std::ostream &os, ClusterEditingInstance &inst);
  
private:
  bool _unweighted;
  bool _dualWeighted;
  bool _realValued;
  
  double _costInsertion;
  double _costDeletion;
  bool _initializedCostInsertion;
  bool _initializedCostDeletion;
  
  lemon::FullGraph _orig;
  lemon::FullGraph::NodeMap<std::string> _nodeNames;
  lemon::FullGraph::EdgeMap<double> _weight;
  lemon::FullGraph::EdgeMap<bool> _permanent;
  lemon::FullGraph::EdgeMap<bool> _forbidden;
  lemon::FullGraph::NodeMap<std::vector<int>* > _clusters;
  
  WorkingCopyInstance* _workingCopyInstance;
};

} // namespace ysk
  
#endif	/* CLUSTEREDITINGINSTANCE_H */
