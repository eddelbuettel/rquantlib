// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
//
// RQuantLib wrapper definitions
//
// Copyright 2014  Dirk Eddelbuettel <edd@debian.org>
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

#ifndef _rquantlib_wrappers_h_
#define _rquantlib_wrappers_h_

#include "rquantlib_internal.h"

// define template specialisations for as and wrap
namespace Rcpp {
    static const unsigned int QLtoJan1970Offset = 25569;  	// Offset to R / Unix epoch 

    template <> QuantLib::Date as(SEXP dtsexp) {
        Rcpp::Date dt(dtsexp);
        return QuantLib::Date(static_cast<int>(dt.getDate()) + QLtoJan1970Offset);
    }

    template <> SEXP wrap(const QuantLib::Date &d) {
        double dt = static_cast<double>(d.serialNumber()); // QL::BigInteger can cast to double
        return Rcpp::wrap(Rcpp::Date(dt - QLtoJan1970Offset));
    }


    // non-intrusive extension via template specialisation
    template <> std::vector<QuantLib::Date> as(SEXP dtvecsexp) {
        Rcpp::DateVector dtvec(dtvecsexp);
        int n = dtvec.size();
        std::vector<QuantLib::Date> dates(n);
        for (int i = 0; i<n; i++){
            dates[i] = QuantLib::Date(static_cast<int>(dtvec[i].getDate()) + QLtoJan1970Offset);
        }
        return dates;
    }

    // non-intrusive extension via template specialisation
    template <> SEXP wrap(const std::vector<QuantLib::Date> &dvec) {
        int n = dvec.size();
        Rcpp::DateVector dtvec(n);
        for (int i = 0; i<n; i++) {
            double dt = static_cast<double>(dvec[i].serialNumber()); // QL::BigInteger can cast to double
            dtvec[i] = Rcpp::Date(dt - QLtoJan1970Offset);
        }
        return Rcpp::wrap(dtvec);
    }

}

#endif
