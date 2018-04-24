#ifndef LIGHTCOMPLETEGRAPH_H
#define LIGHTCOMPLETEGRAPH_H

#include <iostream>
#include <list>
#include <cmath>
#include <set>
#include <limits>

#include "WorkingCopyInstance.h"
#include <lemon/bin_heap.h>

namespace ysk {

class LightCompleteGraph {
  
template<typename T>
using vector = std::vector<T>;

public:
  typedef uint32_t NodeId;
  typedef uint32_t EdgeId;
  typedef double EdgeWeight;
  
  /**
   * Compact data structure to represent an edge. It consists of two node indices.
   */
  struct Edge {
    NodeId u;
    NodeId v;
    
    Edge(NodeId pu, NodeId pv) {
      int ordered = pu < pv;
      u = ordered*pu + (1-ordered)*pv;
      v = ordered*pv + (1-ordered)*pu;
    }
    
    Edge() : u(0), v(1) {};
    
    /**
     * Returns the id of this edge for a triangle adjacency matrix representation.
     */
    EdgeId id() const {
      return v*(v-1)/2 + u;
    }
    
    bool operator==(const Edge& other) const {
      return u == other.u && v == other.v;
    }
  };
  
  static const EdgeWeight Forbidden;
  static const EdgeWeight Permanent;
  static const Edge InvalidEdge;
  static const EdgeId InvalidEdgeId;
  static const NodeId InvalidNodeId;

  /**
   * Constructs a new graph using the provided lemon graph.
   */
  LightCompleteGraph(WorkingCopyInstance& inst);
  
  /**
   * Creates a hard copy of the provided graph.
   */
  LightCompleteGraph(LightCompleteGraph& other);
  
  /**
   * Returns the weight of an edge.
   */
  double getWeight(const Edge e) const;
  
  /**
   * Modifies the weight of an edge.
   */
  void setWeight(const Edge e, const EdgeWeight w);
  
  /**
   * Returns the number of nodes in the graph.
   */
  unsigned int numNodes() const;
  
  /**
   * Returns the number of edges in the graph.
   */
  unsigned long numEdges() const;

  /**
   * Contracts the specified edge, merging the two end nodes together. This will change the internal addresses of the nodes,
   * so the other two methods for addressing (getInternalId and getOriginalIds) should be used.
   */
  void contract(const Edge e);
  
  /**
   * For a node id of the original graph, returns the id of the internal node, which contains the given node.
   */
  NodeId getInternalId(const NodeId v) const;
  
  /**
   * For an internal node id, returns the set of original node ids, which are contained in the internal node. The multiplicity
   * might be greater than 1, if edges have been contracted on this graph.
   */
  vector<NodeId> getOriginalIds(const NodeId v) const;
  
private:
  unsigned int size;
  vector<double> weights;
  vector<NodeId> origToCompr;
  vector<vector<NodeId>> comprToOrig;
};

} //namespace ysk

#endif // LIGHTCOMPLETEGRAPH_H
