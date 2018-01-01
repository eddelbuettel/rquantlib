// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
//
// RQuantLib header
//
// Copyright 2014 - 2018  Dirk Eddelbuettel <edd@debian.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RcppArmadillo.  If not, see <http://www.gnu.org/licenses/>.

#ifndef rquantlib_src_h
#define rquantlib_src_h

// Rcpp Attributes requires a file with package name in inst/include,
// in our case inst/include/RQuantLib.h -- but R does not want us to have
// another header file with the same name, differing only by case
//
// So we place the existing 'rquantlib.h' in src/ instead so that the
// issue of equal names (for everything but the case) no longer
// matters.  This provides us with a backwards compatible
// 'rquantlib.h' file, and its definitons are now in a file
// inst/include/rquantlib_internal.h which we source here.
//
// Moreover, a particular silly company in Cupertino, CA, insists on
// case non-existing for files and file systems, leading to a surprise
// or warning here or there.  So in the current implementation, this file
// exists but is not actually explicitly sourced. Rather, all C++ files
// directly use '#include <rquantlib_internal.h> as well. And for the 
// same reason, and to be totally safe, it is even moved to a directory
// 'deprecated/' below 'src/'. Should you need it, you can move it back.

#include "rquantlib_internal.h"

#endif
