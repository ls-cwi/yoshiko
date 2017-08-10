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

namespace ysk {

/* template <>
 struct std::tr1::hash<std::pair<int, int> > {
 public:
 size_t operator()(std::pair<int, int> x) const throw() {
 size_t h = (x.first * 0x1f1f1f1f) ^ x.second;
 return h;
 }
 };                                            <<<<<< Still relevant or can be removed???
 */

/**
 * Tokenizes a given string into a vector of tokens
 * @param str The string that is to be tokenized
 * @param tokens The string vector that will hold the tokens after processing
 * @param delimiters that are used for splitting (eg. whitespace)
 */
void tokenize(const std::string& str,
              std::vector<std::string>& tokens,
              const std::string& delimiters);

} // namespace ysk

#endif /* HELPERS_H */

