
// RQuantLib wrapper implmenentation
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

#ifndef _rquantlib_impl_h_
#define _rquantlib_impl_h_

#include "rquantlib_internal.h"

// define template specialisations for as and wrap
namespace Rcpp {

    static const unsigned int QLtoJan1970Offset = 25569;  	// Offset to R / Unix epoch

    inline unsigned int getQLtoJan1970offset(void) { return QLtoJan1970Offset; }

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
#if RCPP_VERSION >= Rcpp_Version(0,12,8)
            //dates[i] = QuantLib::Date(static_cast<int>(dtvec[i]) + QLtoJan1970Offset);
            dates[i] = QuantLib::Date(static_cast<int>(Rcpp::Date(dtvec[i]).getDate()) + QLtoJan1970Offset);
#else
            dates[i] = QuantLib::Date(static_cast<int>(dtvec[i].getDate()) + QLtoJan1970Offset);
#endif
        }
        return dates;
    }

    // non-intrusive extension via template specialisation
    template <> SEXP wrap(const std::vector<QuantLib::Date> &dvec) {
        int n = dvec.size();
        Rcpp::DateVector dtvec(n);
        for (int i = 0; i<n; i++) {
            // QL::BigInteger can cast to double
            double dt = static_cast<double>(dvec[i].serialNumber());
            dtvec[i] = Rcpp::Date(dt - QLtoJan1970Offset);
        }
        return Rcpp::wrap(dtvec);
    }

}

#endif
