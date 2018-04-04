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
      int ordered = pu > pv;
      u = ordered*pu + (1-ordered)*pv;
      v = ordered*pv + (1-ordered)*pu;
//       if (pu > pv) {
// 	u = pu;
// 	v = pv;
//       } else {
// 	u = pv;
// 	v = pu;
//       }
    }
    
    Edge() : u(1), v(0) {};
    
    /**
     * Returns the id of this edge for a triangle adjacency matrix representation.
     */
    EdgeId id() const {
      return u*(u-1)/2 + v;
    }
    
    bool operator==(const Edge& other) const {
      return u == other.u && v == other.v;
    }
  };
  
  static const EdgeWeight Forbidden;
  static const EdgeWeight Permanent;
  static const Edge InvalidEdge;
  static const EdgeId InvalidEdgeId;
  
//   const EdgeWeight Forbidden = -std::numeric_limits< EdgeWeight >::infinity();
//   const EdgeWeight Permanent = std::numeric_limits< EdgeWeight >::infinity();
//   const Edge InvalidEdge = {0, 1};
//   const EdgeId InvalidEdgeId = -1;

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
  unsigned int numEdges() const;
  
private:
  unsigned int size;
  vector<double> weights;
};

} //namespace ysk

#endif // LIGHTCOMPLETEGRAPH_H
