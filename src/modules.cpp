// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-

// trying something with Rcpp modules

#include <rquantlib.h>

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
