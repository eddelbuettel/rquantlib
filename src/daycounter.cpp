// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
//
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2009 - 2011  Dirk Eddelbuettel and Khanh Nguyen
//  Copyright (C) 2012 - 2014  Dirk Eddelbuettel
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

#include "rquantlib.h"

QuantLib::DayCounter getDayCounter(const std::string &str)
{
    if (str=="Actual360")
        return QuantLib::Actual360();
    else if (str == "Actual365Fixed")
        return QuantLib::Actual365Fixed();
    else if (str=="ActualActual")
        return QuantLib::ActualActual();
    else
      throw std::runtime_error("Unknown day-counter: " + str);
}

// [[Rcpp::interfaces(r, cpp)]]

// Global implied day-counters
QuantLib::DayCounter __dc__ = QuantLib::Actual360();

// [[Rcpp::export]]
bool setDayCount(std::string str)
{
    // Set it to a global variable such that other C++ functions can access it
    __dc__ = getDayCounter(str);

    return true;
}

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

