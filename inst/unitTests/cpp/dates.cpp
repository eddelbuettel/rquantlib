// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
//
// dates.cpp: RQuantLib date conversion tests
//
// Copyright (C) 2014 - 2016  Dirk Eddelbuettel
//
// This file is part of RQuantLib.
//
// RQuantLib is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// RQuantLib is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RQuantLib.  If not, see <http://www.gnu.org/licenses/>.


// [[Rcpp::depends(RQuantLib)]]

#include <RQuantLib.h>

// [[Rcpp::export]]
Rcpp::Date advanceDateRR(Rcpp::Date d, int n) {
    return d + n;
}

// [[Rcpp::export]]
Rcpp::Date advanceDateQR(QuantLib::Date d, int n) {
    return Rcpp::wrap(d + n);
}

// [[Rcpp::export]]
QuantLib::Date advanceDateQQ(QuantLib::Date d, int n) {
    return d + n;
}

// [[Rcpp::export]]
Rcpp::DateVector advanceDatesRR(Rcpp::DateVector d, int n) {
    Rcpp::DateVector nd(d.size());
    for (int i=0; i<d.size(); i++) nd[i] = d[i] + n;
    return nd;
}

// [[Rcpp::export]]
Rcpp::DateVector advanceDatesQR(std::vector<QuantLib::Date> d, int n) {
    for (unsigned int i=0; i<d.size(); i++) d[i] = d[i] + n;
    return Rcpp::wrap(d);
}

// [[Rcpp::export]]
std::vector<QuantLib::Date> advanceDatesQQ(std::vector<QuantLib::Date> d, int n) {
    for (unsigned int i=0; i<d.size(); i++) d[i] = d[i] + n;
    return d;
}



