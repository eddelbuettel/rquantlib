
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2014         Michele Salvadore and Dirk Eddelbuettel
//  Copyright (C) 2015 - 2018  Dirk Eddelbuettel
//
//  This file is part of RQuantLib.
//
//  RQuantLib is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  RQuantLib is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with RQuantLib.  If not, see <http://www.gnu.org/licenses/>.


//  [[Rcpp::interfaces(r, cpp)]]

#include <rquantlib_internal.h>

// [[Rcpp::export]]
Rcpp::DateVector CreateSchedule(Rcpp::List params) {

    QuantLib::Schedule schedule = getSchedule(params);
    return Rcpp::wrap(schedule.dates());
}
