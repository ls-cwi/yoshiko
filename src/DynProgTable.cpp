/*
 * File:   DynProgTable.cpp
 * Author: emanuellaude
 *
 * Created on 13. September 2012, 13:05
 */

#include "DynProgTable.h"

using namespace std;

namespace ysk {

DynProgTable::DynProgTable (const DynProgTable &copy){
	_table = copy._table;
	_length = copy._length;
}


DynProgTable::DynProgTable(int bound) : _length(2*bound+1) {
  _table = new double*[2];
  _table[0] = new double[_length];
  _table[1] = new double[_length];
}

DynProgTable::~DynProgTable() {
  for(int i = 0; i < 2; i++) {
    delete [] _table[i];
  }
  delete [] _table;
}

double DynProgTable::getValue(int x) {
  int z = (x >= 0 ? (2 * x) : (-2 * x -1));
  if(z >= _length) {
    cerr << "array index out of bounds"<<endl;
    exit(-1);
  }
  
  return _table[0][z];
}

void DynProgTable::setValue(int x, double value) {
  int z = (x >= 0 ? (2 * x) : (-2 * x -1));
  if(z >= _length) {
    cerr << "array index out of bounds" << endl;
    exit(-1);
  }
  _table[1][z] = value;
}

void DynProgTable::swap() {
  delete [] _table[0];
  _table[0] = _table[1];
  _table[1] = new double[_length];
}

//void DynProgTable::print() {
//  int bound = (_length-1)/2;
//  for(int x = -bound; x <= bound; x++) {
//    cout << getValue(x)<<"\t";
//  }
//  cout <<endl;
//}

} // namespace ysk
