
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2002 - 2024  Dirk Eddelbuettel <edd@debian.org>
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

#include <rquantlib_internal.h>

// [[Rcpp::interfaces(r, cpp)]]

// [[Rcpp::export]]
Rcpp::List europeanOptionEngine(std::string type,
                                double underlying,
                                double strike,
                                double dividendYield,
                                double riskFreeRate,
                                double maturity,
                                double volatility,
                                Rcpp::Nullable<Rcpp::NumericVector> discreteDividends,
                                Rcpp::Nullable<Rcpp::NumericVector> discreteDividendsTimeUntil) {

    QuantLib::Option::Type optionType = getOptionType(type);
    QuantLib::Date today = QuantLib::Date::todaysDate();
    QuantLib::Date exDate = getFutureDate(today, maturity);
    QuantLib::Settings::instance().evaluationDate() = today;
    QuantLib::DayCounter dc = QuantLib::Actual360();
    auto spot  = qlext::make_shared<QuantLib::SimpleQuote>(underlying);
    auto vol   = qlext::make_shared<QuantLib::SimpleQuote>(volatility);
    auto volTS = flatVol(today, vol, dc); 		// cf src/utils.cpp
    auto qRate = qlext::make_shared<QuantLib::SimpleQuote>(dividendYield);
    auto qTS   = flatRate(today, qRate, dc); 	// cf src/utils.cpp
    auto rRate = qlext::make_shared<QuantLib::SimpleQuote>(riskFreeRate);
    auto rTS   = flatRate(today, rRate, dc); 	// cf src/utils.cpp

    bool withDividends = discreteDividends.isNotNull() && discreteDividendsTimeUntil.isNotNull();

    auto exercise = qlext::make_shared<QuantLib::EuropeanExercise>(exDate);
    auto payoff   = qlext::make_shared<QuantLib::PlainVanillaPayoff>(optionType, strike);

    if (withDividends) {
        Rcpp::NumericVector divvalues(discreteDividends), divtimes(discreteDividendsTimeUntil);
        int n = divvalues.size();
        std::vector<QuantLib::Date> discDivDates(n);
        std::vector<double> discDividends(n);
        for (int i = 0; i < n; i++) {
            discDivDates[i] = getFutureDate(today, divtimes[i]);
            discDividends[i] = divvalues[i];
        }

        typedef QuantLib::BlackScholesMertonProcess qlBSMp;
        auto stochProcess = qlext::make_shared<qlBSMp>(QuantLib::Handle<QuantLib::Quote>(spot),
                                                       QuantLib::Handle<QuantLib::YieldTermStructure>(qTS),
                                                       QuantLib::Handle<QuantLib::YieldTermStructure>(rTS),
                                                       QuantLib::Handle<QuantLib::BlackVolTermStructure>(volTS));

        auto engine = qlext::make_shared<QuantLib::AnalyticDividendEuropeanEngine>(stochProcess, QuantLib::DividendVector(discDivDates, discDividends));

        QuantLib::VanillaOption option(payoff, exercise);
        option.setPricingEngine(engine);

        return Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                  Rcpp::Named("delta") = option.delta(),
                                  Rcpp::Named("gamma") = option.gamma(),
                                  Rcpp::Named("vega") = option.vega(),
                                  Rcpp::Named("theta") = option.theta(),
                                  Rcpp::Named("rho") = option.rho(),
                                  Rcpp::Named("divRho") = R_NaReal);
    }
    else {

        auto option = makeOption(payoff, exercise, spot, qTS, rTS, volTS);
        return Rcpp::List::create(Rcpp::Named("value")  = option->NPV(),
                                  Rcpp::Named("delta")  = option->delta(),
                                  Rcpp::Named("gamma")  = option->gamma(),
                                  Rcpp::Named("vega")   = option->vega(),
                                  Rcpp::Named("theta")  = option->theta(),
                                  Rcpp::Named("rho")    = option->rho(),
                                  Rcpp::Named("divRho") = option->dividendRho());
    }

}

// [[Rcpp::export]]
Rcpp::List americanOptionEngine(std::string type,
                                double underlying,
                                double strike,
                                double dividendYield,
                                double riskFreeRate,
                                double maturity,
                                double volatility,
                                int timeSteps,
                                int gridPoints,
                                std::string engine,
                                Rcpp::Nullable<Rcpp::NumericVector> discreteDividends,
                                Rcpp::Nullable<Rcpp::NumericVector> discreteDividendsTimeUntil) {

    QuantLib::Option::Type optionType = getOptionType(type);

    QuantLib::Date today = QuantLib::Date::todaysDate();
    QuantLib::Settings::instance().evaluationDate() = today;
    QuantLib::Date exDate = getFutureDate(today, maturity);
    QuantLib::DayCounter dc = QuantLib::Actual360();
    auto spot  = qlext::make_shared<QuantLib::SimpleQuote>(underlying);
    auto qRate = qlext::make_shared<QuantLib::SimpleQuote>(dividendYield);
    auto qTS   = flatRate(today, qRate, dc); 	// cf src/utils.cpp
    auto rRate = qlext::make_shared<QuantLib::SimpleQuote>(riskFreeRate);
    auto rTS   = flatRate(today, rRate, dc); 	// cf src/utils.cpp
    auto vol   = qlext::make_shared<QuantLib::SimpleQuote>(volatility);
    auto volTS = flatVol(today, vol, dc); 		// cf src/utils.cpp

    bool withDividends = discreteDividends.isNotNull() && discreteDividendsTimeUntil.isNotNull();

    auto exercise = qlext::make_shared<QuantLib::AmericanExercise>(today, exDate);
    auto payoff   = qlext::make_shared<QuantLib::PlainVanillaPayoff>(optionType, strike);

    typedef QuantLib::BlackScholesMertonProcess qlBSMp;
    auto stochProcess = qlext::make_shared<qlBSMp>(QuantLib::Handle<QuantLib::Quote>(spot),
                                                   QuantLib::Handle<QuantLib::YieldTermStructure>(qTS),
                                                   QuantLib::Handle<QuantLib::YieldTermStructure>(rTS),
                                                   QuantLib::Handle<QuantLib::BlackVolTermStructure>(volTS));

    if (withDividends) {
        Rcpp::NumericVector divvalues(discreteDividends), divtimes(discreteDividendsTimeUntil);
        int n = divvalues.size();
        std::vector<QuantLib::Date> discDivDates(n);
        std::vector<double> discDividends(n);
        for (int i = 0; i < n; i++) {
            discDivDates[i] = getFutureDate(today, divtimes[i]);
            discDividends[i] = divvalues[i];
        }

        QuantLib::VanillaOption option(payoff, exercise);

        if (engine=="BaroneAdesiWhaley") {
            Rcpp::warning("Discrete dividends, engine switched to CrankNicolson");
            engine = "CrankNicolson";
        }

        if (engine=="CrankNicolson") { // FDDividendAmericanEngine only works with CrankNicolson
            // see eg test-suite/americanoption.cc
            option.setPricingEngine(QuantLib::MakeFdBlackScholesVanillaEngine(stochProcess).withCashDividends(discDivDates, discDividends));
            return Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                      Rcpp::Named("delta") = option.delta(),
                                      Rcpp::Named("gamma") = option.gamma(),
                                      Rcpp::Named("vega") = R_NaReal,
                                      Rcpp::Named("theta") = R_NaReal,
                                      Rcpp::Named("rho") = R_NaReal,
                                      Rcpp::Named("divRho") = R_NaReal);
        } else {
            throw std::range_error("Unknown engine " + engine);
        }

    } else {
        QuantLib::VanillaOption option(payoff, exercise);

        if (engine=="BaroneAdesiWhaley") {
            // new from 0.3.7 BaroneAdesiWhaley
            auto engine = qlext::make_shared<QuantLib::BaroneAdesiWhaleyApproximationEngine>(stochProcess);
            option.setPricingEngine(engine);
            return Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                      Rcpp::Named("delta") = R_NaReal,
                                      Rcpp::Named("gamma") = R_NaReal,
                                      Rcpp::Named("vega") = R_NaReal,
                                      Rcpp::Named("theta") = R_NaReal,
                                      Rcpp::Named("rho") = R_NaReal,
                                      Rcpp::Named("divRho") = R_NaReal);
        } else if (engine=="CrankNicolson") {
            // suggestion by Bryan Lewis: use CrankNicolson for greeks (but now down to delta + gamma)
            auto engine = qlext::make_shared<QuantLib::FdBlackScholesVanillaEngine>(stochProcess, timeSteps, gridPoints);
            option.setPricingEngine(engine);
            return Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                      Rcpp::Named("delta") = option.delta(),
                                      Rcpp::Named("gamma") = option.gamma(),
                                      Rcpp::Named("vega") = R_NaReal,
                                      Rcpp::Named("theta") = R_NaReal,
                                      Rcpp::Named("rho") = R_NaReal,
                                      Rcpp::Named("divRho") = R_NaReal);
        } else {
            throw std::range_error("Unknown engine " + engine);
        }
    }


}

// [[Rcpp::export]]
Rcpp::List europeanOptionArraysEngine(std::string type, Rcpp::NumericMatrix par) {

    QuantLib::Option::Type optionType = getOptionType(type);
    int n = par.nrow();
    Rcpp::NumericVector value(n), delta(n), gamma(n), vega(n), theta(n), rho(n), divrho(n);

    QuantLib::Date today = QuantLib::Date::todaysDate();
    QuantLib::Settings::instance().evaluationDate() = today;

    QuantLib::DayCounter dc = QuantLib::Actual360();

    for (int i=0; i<n; i++) {

        double underlying    = par(i, 0);    // first column
        double strike        = par(i, 1);    // second column
        QuantLib::Spread dividendYield = par(i, 2);    // third column
        QuantLib::Rate riskFreeRate    = par(i, 3);    // fourth column
        QuantLib::Time maturity        = par(i, 4);    // fifth column
        double volatility    = par(i, 5);    // sixth column

        auto spot  = qlext::make_shared<QuantLib::SimpleQuote>(underlying);
        auto vol   = qlext::make_shared<QuantLib::SimpleQuote>(volatility);
        auto volTS = flatVol(today, vol, dc); 		// cf src/utils.cpp
        auto qRate = qlext::make_shared<QuantLib::SimpleQuote>(dividendYield);
        auto qTS   = flatRate(today, qRate, dc); 	// cf src/utils.cpp
        auto rRate = qlext::make_shared<QuantLib::SimpleQuote>(riskFreeRate);
        auto rTS   = flatRate(today, rRate, dc); 	// cf src/utils.cpp

        QuantLib::Date exDate = getFutureDate(today, maturity);
        auto exercise = qlext::make_shared<QuantLib::EuropeanExercise>(exDate);
        auto payoff   = qlext::make_shared<QuantLib::PlainVanillaPayoff>(optionType, strike);
        auto option = makeOption(payoff, exercise, spot, qTS, rTS, volTS);

        value[i]  = option->NPV();
        delta[i]  = option->delta();
        gamma[i]  = option->gamma();
        vega[i]   = option->vega();
        theta[i]  = option->theta();
        rho[i]    = option->rho();
        divrho[i] = option->dividendRho();
    }
    return Rcpp::List::create(Rcpp::Named("value")  = value,
                              Rcpp::Named("delta")  = delta,
                              Rcpp::Named("gamma")  = gamma,
                              Rcpp::Named("vega")   = vega,
                              Rcpp::Named("theta")  = theta,
                              Rcpp::Named("rho")    = rho,
                              Rcpp::Named("divRho") = divrho);
}
