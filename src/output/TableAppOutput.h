#ifndef TABLEAPPOUTPUT_H
#define TABLEAPPOUTPUT_H

#include <iostream>
#include <string>

#include "TableOutput.h"

namespace ysk {

class TableAppOutput : public TableOutput {
public:
  TableAppOutput(ClusterEditingInstance& inst,
                 ClusterEditingSolutions& solutions,
                 std::string filename,
                 std::string suffix,
                 std::string label)
    : TableOutput(inst, solutions, filename, suffix, label)
  {
  }

  void openStream(size_t solution);
};

} // namespace ysk

#endif /* TABLEAPPOUTPUT_H */
