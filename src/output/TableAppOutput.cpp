#include "../output/TableAppOutput.h"

using namespace std;

namespace ysk {

void TableAppOutput::openStream(size_t solution) {
  _os.open (_filename.c_str());
  
  if(!_os.is_open()) {
    cerr<< "Error: Cannot create/open file: "<< _filename <<endl;
    exit(-1);
  }
}

} // namespace ysk
