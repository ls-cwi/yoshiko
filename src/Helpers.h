/*
 *  helpers.h
 *  yoshiko
 *
 *  Created by Gunnar W. Klau on 6/15/11.
 *  Copyright 2011 Centrum Wiskunde & Informatica (CWI). All rights reserved.
 *
 */

#ifndef HELPERS_H
#define HELPERS_H


#include <iostream>
#include <fstream>
#include <vector>


/* template <>
 struct std::tr1::hash<std::pair<int, int> > {
 public:
 size_t operator()(std::pair<int, int> x) const throw() {
 size_t h = (x.first * 0x1f1f1f1f) ^ x.second;
 return h;
 }
 };
 */

void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);


#endif /* HELPERS_H */

