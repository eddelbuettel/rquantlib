
#include <rquantlib.h>

// [[Rcpp::interfaces(r, cpp)]]

// [[Rcpp::export]]
QuantLib::Date advanceDate(QuantLib::Date issueDate, int days) {
    // TODO:: calendar as argument, or maybe use calendar from singleton...
    QuantLib::Calendar cal = QuantLib::UnitedStates(QuantLib::UnitedStates::GovernmentBond);
    QuantLib::Date newDate = cal.advance(issueDate, days, QuantLib::Days);
    return(newDate);
}

