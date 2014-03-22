
#ifndef _rquantlib_wrappers_h_
#define _rquantlib_wrappers_h_

#include "rquantlib.h"

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
}

#endif
