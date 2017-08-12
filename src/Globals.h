//
//  globals.h
//  yoshiko
//
//  Created by Gunnar Klau on 15-06-12.
//  Copyright (c) 2012 Centrum Wiskunde & Informatica (CWI). All rights reserved.
//

#ifndef GLOBALS_H
#define GLOBALS_H

#include <limits>
#include <cstdlib>

#include <lemon/time_measure.h>


namespace ysk {

	extern int time_limit;
	extern lemon::Timer clk;
	extern int verbosity; //<<< TODO: CONVERT TO ENUM
	extern int no_threads;
	extern double eps;

	enum EdgeType{
				UNDECIDED = 0,
				PERMANENT = 1,
				FORBIDDEN = 2
	};

} // namespace ysk
  
#endif /* GLOBALS_H */

