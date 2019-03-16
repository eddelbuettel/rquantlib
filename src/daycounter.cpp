
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2009 - 2011  Dirk Eddelbuettel and Khanh Nguyen
//  Copyright (C) 2012 - 2018  Dirk Eddelbuettel
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

#include <rquantlib_internal.h>

// [[Rcpp::interfaces(r, cpp)]]

// [[Rcpp::export]]
std::vector<double> dayCount(std::vector<QuantLib::Date> startDates,
                             std::vector<QuantLib::Date> endDates,
                             std::vector<double> dayCounters) {

    int n = dayCounters.size();
    std::vector<double> result(n);
    for (int i=0; i< n; i++){
        QuantLib::DayCounter counter = getDayCounter(dayCounters[i]);
        result[i] = static_cast<double>(counter.dayCount(startDates[i], endDates[i]));
    }
    return result;
}


// [[Rcpp::export]]
std::vector<double> yearFraction(std::vector<QuantLib::Date> startDates,
                                 std::vector<QuantLib::Date> endDates,
                                 std::vector<double> dayCounters) {

    int n = dayCounters.size();
    std::vector<double> result(n);
    for (int i=0; i< n; i++){
        QuantLib::DayCounter counter = getDayCounter(dayCounters[i]);
        result[i] = (double)counter.yearFraction(startDates[i], endDates[i]);
    }
    return result;
}

// this could go into another file too... maybe regroup all calendar / date functions?
// [[Rcpp::export]]
bool setEvaluationDate(QuantLib::Date evalDate) {
    QuantLib::Settings::instance().evaluationDate() = evalDate;
    return true;
}
