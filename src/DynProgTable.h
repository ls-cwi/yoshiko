/*
 * File:   DynProgTable.h
 * Author: emanuellaude
 *
 * Created on 13. September 2012, 13:05
 */

#ifndef DYNPROGTABLE_H
#define	DYNPROGTABLE_H

#include <iostream>
#include <cstdlib>

namespace ysk {
  
class DynProgTable {

	public:
		DynProgTable (const DynProgTable &copy);
		DynProgTable(int bound);
		double getValue(int x);
		void setValue(int x, double value);
		void swap();
		//void print();
		~DynProgTable();

	private:
		double** _table;
		int _length;
};
  
} // namespace ysk

#endif	/* DYNPROGTABLE_H */

