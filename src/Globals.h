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


extern int time_limit;
extern lemon::Timer clk;
extern int verbosity;
extern int no_threads;
extern double eps;

#endif /* GLOBALS_H */

