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

	/**
	 * The time limit in seconds, can be set globally
	 * Note: This is currently only respected by the ILP, the heuristic doesn't care about it and the reduction rules don't take it into account either
	 */
	extern int time_limit;

	extern double threshold;

	extern lemon::Timer clk;
	extern int verbosity;
	extern int no_threads;
	extern double eps;

	extern bool isTerminated;

	enum EdgeType{
		UNDECIDED = 0,
		PERMANENT = 1,
		FORBIDDEN = 2
	};


} // namespace ysk

#endif /* GLOBALS_H */

