/*
 * File:   CriticalCliqueRule.h
 * Author: emanuellaude
 *
 * Created on 26. August 2012, 12:08
 */

#ifndef CRITICALCLIQUERULE_H
#define	CRITICALCLIQUERULE_H

#include <lemon/full_graph.h>
#include <lemon/adaptors.h>
#include <lemon/list_graph.h>

#include <iostream>
#include <vector>

#include "WorkingCopyInstance.h"
#include "FPTReductionRule.h"

class CriticalCliqueRule : public FPTReductionRule {
public:
  CriticalCliqueRule(WorkingCopyInstance& inst) : FPTReductionRule(inst, "Critical Clique Rule", true),
  _neighborhood(_temp), _criticalClique(_temp) {
  }
  void apply();
  
  virtual ~CriticalCliqueRule();
private:
  void computeClosedNeighborhood(std::vector<int>* neighborhood, WorkingCopyGraph::Node& u);
  void mergeCriticalCliques();
  
  //temporary graph to store/compute critical cliques and neighborhoods
  lemon::ListGraph _temp;
  lemon::ListGraph::NodeMap <std::vector<int>* > _neighborhood;
  lemon::ListGraph::NodeMap <std::vector<WorkingCopyGraph::Node>* > _criticalClique;
};

#endif	/* CRITICALCLIQUERULE_H */

