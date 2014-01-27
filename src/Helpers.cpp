/*
 *  Helpers.cpp
 *  yoshiko
 *
 *  Created by Gunnar W. Klau on 6/15/11.
 *  Copyright 2011 Centrum Wiskunde & Informatica (CWI). All rights reserved.
 *
 */

#include "Helpers.h"

using namespace std;

void tokenize(const string& str, vector<string>& tokens, const string& delimiters) {
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
    
    while (string::npos != pos || string::npos != lastPos) {
        // Found a token, add it to the vector.
        tokens.push_back((str.substr(lastPos, pos - lastPos)).c_str());
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
