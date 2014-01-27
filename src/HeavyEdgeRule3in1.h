/*
 * File:   HeavyEdgeRule3in1.h
 * Author: emanuellaude
 *
 * Created on 27. August 2012, 14:24
 */

#ifndef HEAVYEDGERULE3IN1_H
#define	HEAVYEDGERULE3IN1_H

#include <lemon/full_graph.h>
#include <lemon/adaptors.h>
#include <lemon/lgf_writer.h>

#include <cmath>
#include <list>
#include <limits>

#include "WorkingCopyInstance.h"
#include "FPTReductionRule.h"


class HeavyEdgeRule3in1 : public FPTReductionRule {
public:
  HeavyEdgeRule3in1(WorkingCopyInstance& inst, bool conserveMultipleSolutions) : FPTReductionRule(inst, "Heavy Edge Rule 3 in 1", conserveMultipleSolutions), _r1(inst.getGraph()), _r2(inst.getGraph()), _r3(inst.getGraph(), -std::numeric_limits<double>::infinity()) {
  }
  virtual ~HeavyEdgeRule3in1();
  
  void apply();
  
private:
  void init();
  void computeCosts(WorkingCopyGraph::Edge& e);
  void setForbidden(WorkingCopyGraph::Edge& uv);
  void setPermanent(WorkingCopyGraph::Edge& uv);
  void beforeSetForbidden(WorkingCopyGraph::Edge& uv);
  void beforeSetPermanent(WorkingCopyGraph::Edge uv);
  void afterSetPermanent(const WorkingCopyGraph::Edge &uv, WorkingCopyGraph::Node &proxy);
  
  WorkingCopyGraph::NodeMap<WorkingCopyGraph::NodeMap<double>* > _r1;
  WorkingCopyGraph::NodeMap<WorkingCopyGraph::NodeMap<double>* > _r2;
  WorkingCopyGraph::EdgeMap<double> _r3;
  
  std::list<WorkingCopyGraph::Edge> _forbidden;
  std::list<WorkingCopyGraph::Edge> _permanent;
};

#endif	/* HEAVYEDGERULE3IN1_H */


