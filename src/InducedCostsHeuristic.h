#ifndef INDUCEDCOSTSHEURISTIC_H
#define INDUCEDCOSTSHEURISTIC_H

#include <iostream>

#include <lemon/full_graph.h>
#include <lemon/lgf_writer.h>

#include "WorkingCopyInstance.h"
#include "ParameterizedInstance.h"
#include "ClusterEditingInstance.h"
#include "ClusterEditingSolutions.h"

namespace ysk {

class InducedCostsHeuristic {
public:
  InducedCostsHeuristic(ParameterizedInstance& inst)
    : _upperBound(0)
    , _instance()
    , _parameterizedInstance(NULL)
  {
    init(inst);
  }

  virtual ~InducedCostsHeuristic();
  double getUpperBound();
  /**
   * Applies the heuristic and saves the result as a CES in the given instance.
   * Note that this requires a previous initialization of this class via ParametrizedInstance
   * @param solution The solution as CES that is to be used as a target
   * @return The cost of applying the heuristic as double
   */
  double getSolution(ClusterEditingSolutions& solution);
  void start();

private:
  void init(ParameterizedInstance& inst);

  double _upperBound;
  ClusterEditingInstance _instance;
  ParameterizedInstance* _parameterizedInstance;
};

} // namespace ysk

#endif /* INDUCEDCOSTSHEURISTIC_H */
