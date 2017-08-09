%module ClusterEditingInstance

%ignore ~ClusterEditingInstance();
%ignore init(int size);
%ignore getWorkingCopyInstance();
%ignore getOrig() const;
%ignore computeCost() const;
%ignore getWeight(const lemon::FullGraph::Edge& e) const;

%ignore isUnweighted() const;

%ignore isDualWeighted() const;

%ignore isRealValued() const;

%ignore getEdgeName(const lemon::FullGraph::Edge e) const;

%ignore getNodeName(const lemon::FullGraph::Node node) const;

%ignore isPermanent(const lemon::FullGraph::Edge& e) const;

%ignore isForbidden(const lemon::FullGraph::Edge& e) const;

%ignore isDirty() const;

%ignore parseJenaFormat(std::istream &is);

%ignore parseCleverFormat(std::istream &is);

%ignore parseSIFFormat(std::istream &is);

%ignore getCluster(lemon::FullGraph::Node u) const;

%ignore operator <<(std::ostream &os, ClusterEditingInstance &inst);

%include "ClusterEditingInstance.h"
