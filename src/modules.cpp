
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2013 - 2018  Dirk Eddelbuettel
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

// trying something with Rcpp modules -- still experimental

#include <rquantlib_internal.h>

using namespace QuantLib;

Real BlackFormula(std::string type, Real strike, Real fwd, Real stdDev, Real discount, Real displacement) {
    if (type=="call")
        return blackFormula(Option::Call, strike, fwd, stdDev, discount, displacement);
    else if (type=="put")
        return blackFormula(Option::Put, strike, fwd, stdDev, discount, displacement);
    else {
        Rcpp::stop("Unrecognised option type");
        return(-42);            // never reached
    }
}

Real BlackFormulaImpliedStdDevApproximation(std::string type, Real strike, Real fwd, Real blackPrice,
                                            Real discount, Real displacement) {
    if (type=="call")
        return blackFormulaImpliedStdDevApproximation(Option::Call, strike, fwd, blackPrice, discount, displacement);
    else if (type=="put")
        return blackFormulaImpliedStdDevApproximation(Option::Put, strike, fwd, blackPrice, discount, displacement);
    else {
        Rcpp::stop("Unrecognised option type");
        return(-42);            // never reached
    }
}

RCPP_MODULE(BlackMod) {

    using namespace Rcpp;

    function("BlackFormula",   			// name of the identifier at the R level
             &BlackFormula,       		// function pointer to helper function defined above
             List::create(Named("type")         = "character",
                          Named("strike")       = "numeric",    // function arguments including default value
                          Named("fwd")          = "numeric",
                          Named("stddev")       = "numeric",
                          Named("discount")     = 1.0,  // cf ql/pricingengines/blackformula.hpp
                          Named("displacement") = 0.0), // cf ql/pricingengines/blackformula.hpp
             "Black (1976) formula for an option [note that stdev=vol*sqrt(timeToExp)]");

    function("BlackFormulaImpliedStdDevApproximation",   		// name of the identifier at the R level
             &BlackFormulaImpliedStdDevApproximation,       		// function pointer to helper function defined above
             List::create(Named("type")         = "character",
                          Named("strike")       = "numeric",    // function arguments including default value
                          Named("fwd")          = "numeric",
                          Named("blackPrice")   = "numeric",
                          Named("discount")     = 1.0,  // cf ql/pricingengines/blackformula.hpp
                          Named("displacement") = 0.0), // cf ql/pricingengines/blackformula.hpp
             "Approximated Black 1976 implied standard deviation, i.e. volatility*sqrt(timeToMaturityBlack");

    // also see blackFormulaImpliedStdDev()

}



class Bonds;
RCPP_EXPOSED_CLASS(Bonds)
RCPP_MODULE(BondsMod) {
    Rcpp::class_<Bond>("Bond")
        ;
}
