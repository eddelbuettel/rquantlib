// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2009 - 2011  Dirk Eddelbuettel and Khanh Nguyen
// Copyright (C) 2012 - 2014  Dirk Eddelbuettel
//
// This file is part of the RQuantLib library for GNU R.
// It is made available under the terms of the GNU General Public
// License, version 2, or at your option, any later version,
// incorporated herein by reference.
//
// This program is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free
// Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA

#include "rquantlib.h"

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
