// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2009 - 2010  Dirk Eddelbuettel and Khanh Nguyen
//
// $Id$
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

#include "rquantlib.hpp"

using namespace boost;

RcppExport SEXP dayCount(SEXP startDates, SEXP endDates, SEXP dayCounter){

    try {
        
        Rcpp::DateVector s = Rcpp::DateVector(startDates);
        Rcpp::DateVector e = Rcpp::DateVector(endDates);
        
		Rcpp::NumericVector dc(dayCounter);
        
        int n = dc.size();
        std::vector<double> result(n);
        for (int i=0; i< n; i++){
            QuantLib::Date d1( dateFromR(s[i]) );
            QuantLib::Date d2( dateFromR(e[i]) );
            DayCounter counter = getDayCounter(dc[i]);
            result[i] = static_cast<double>(counter.dayCount(d1, d2));            
        }
        return Rcpp::wrap(result);
        
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}


RcppExport SEXP yearFraction(SEXP startDates, SEXP endDates, SEXP dayCounter){

    try {
        
        Rcpp::DateVector s = Rcpp::DateVector(startDates);
        Rcpp::DateVector e = Rcpp::DateVector(endDates);
        
		Rcpp::NumericVector dc(dayCounter);
        int n = dc.size();
        std::vector<double> result(n);
        for (int i=0; i< n; i++){
            QuantLib::Date d1( dateFromR(s[i]) );
            QuantLib::Date d2( dateFromR(e[i]) );            
            DayCounter counter = getDayCounter(dc[i]);
            result[i] = (double)counter.yearFraction(d1, d2);            
        }        
        return Rcpp::wrap(result);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}
