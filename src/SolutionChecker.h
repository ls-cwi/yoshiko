#ifndef SOLUTIONCHECKER_H
#define SOLUTIONCHECKER_H

#include "ClusterEditingInstance.h"
#include "ClusterEditingSolutions.h"

#include <lemon/adaptors.h>
#include <lemon/lgf_writer.h>
#include <lemon/full_graph.h>

#include <set>

namespace ysk{
    
class SolutionChecker
{
public:
    static const int MARGIN_OF_ERROR = 1;
    
    static bool verifySolutionIsCliqueGraph(ClusterEditingInstance & instance,  ClusterEditingSolutions & solutions);
    static bool verifySolutionCosts(ClusterEditingInstance & instance, ClusterEditingSolutions & solution);
};

}

#endif // SOLUTIONCHECKER_H
