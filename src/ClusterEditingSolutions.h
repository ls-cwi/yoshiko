#ifndef CLUSTEREDITINGSOLUTION_H
#define CLUSTEREDITINGSOLUTION_H

#include "ClusterEditingInstance.h"
#include "Globals.h"
#include "Helpers.h"
#include "SolutionFlags.h"
#include "WorkingCopyInstance.h"

#include <ilconcert/iloalg.h>

#include <lemon/list_graph.h>
#include <lemon/connectivity.h>

#include <vector>

namespace ysk {

///
class ClusterEditingSolutions {
public:

  size_t getNumberOfClusters(size_t i) const;

  std::vector<int>& getCluster(size_t i, size_t k);

  std::vector<std::vector<int> >& getSolution(size_t i);

  void resize(long numberOfSolutions);


  //Setter/Getter methods

  void setSolution(int k,
                   const IloNumArray &x_vals,
                   const ClusterEditingInstance &inst);

  void setSolution(int k,
                   const ClusterEditingInstance& inst);

  void setSolution(int k,
                   const WorkingCopyInstance& inst);

  /**
   * Returns the number of solutions (with equal cost) that this object contains
   */
  size_t getNumberOfSolutions() const;

  ysk::SolutionFlags getFlags();
  void setFlags(SolutionFlags f);

  void printSolution(size_t index);

private:
  // the partitions (multiple because of possible multiple optimal solutions)
  std::vector<std::vector<std::vector<int> > > _solutions;
  SolutionFlags _flags;
};

} // namespace ysk

#endif /* CLUSTEREDITINGSOLUTION_H */
