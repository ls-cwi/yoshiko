//
//  TableAppOutput.cpp
//  weighted-cluster-editing
//
//  Created by Emanuel Laude on 21.03.13.
//  Copyright (c) 2013 Emanuel Laude. All rights reserved.
//

#include "TableAppOutput.h"

using namespace std;

void TableAppOutput::openStream(size_t solution) {
    _os.open (_filename.c_str());

    if(!_os.is_open()) {
        cerr<< "Error: Cannot create/open file: "<< _filename <<endl;
        exit(-1);
    }
}
