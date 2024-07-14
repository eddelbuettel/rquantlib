
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

// [[Rcpp::export]]
Rcpp::List binaryOptionEngine(std::string binType,
                              std::string type,
                              std::string excType,
                              double underlying,
                              double strike,
                              double dividendYield,
                              double riskFreeRate,
                              double maturity,
                              double volatility,
                              double cashPayoff) {

    QuantLib::Option::Type optionType = getOptionType(type);

    // new QuantLib 0.3.5 framework: digitals, updated for 0.3.7
    // updated again for QuantLib 0.9.0,
    // cf QuantLib-0.9.0/test-suite/digitaloption.cpp
    QuantLib::Date today = QuantLib::Date::todaysDate();
    QuantLib::Settings::instance().evaluationDate() = today;
    QuantLib::Date exDate = getFutureDate(today, maturity);

    QuantLib::DayCounter dc = QuantLib::Actual360();
    auto spot  = qlext::make_shared<QuantLib::SimpleQuote>(underlying);
    auto qRate = qlext::make_shared<QuantLib::SimpleQuote>(dividendYield);
    auto qTS   = flatRate(today,qRate,dc);
    auto rRate = qlext::make_shared<QuantLib::SimpleQuote>(riskFreeRate);
    auto rTS   = flatRate(today,rRate,dc);
    auto vol   = qlext::make_shared<QuantLib::SimpleQuote>(volatility);
    auto volTS = flatVol(today, vol, dc);

    QuantLib::ext::shared_ptr<QuantLib::StrikedTypePayoff> payoff;
    if (binType=="cash") {
        payoff = qlext::make_shared<QuantLib::CashOrNothingPayoff>(optionType, strike, cashPayoff);
    } else if (binType=="asset") {
        payoff = qlext::make_shared<QuantLib::AssetOrNothingPayoff>(optionType, strike);
    } else if (binType=="gap") {
        payoff = qlext::make_shared<QuantLib::GapPayoff>(optionType, strike, cashPayoff);
    } else {
        throw std::range_error("Unknown binary option type " + binType);
    }

    QuantLib::ext::shared_ptr<QuantLib::Exercise> exercise;
    if (excType=="american") {
        exercise = qlext::make_shared<QuantLib::AmericanExercise>(today, exDate);
    } else if (excType=="european") {
        exercise = qlext::make_shared<QuantLib::EuropeanExercise>(exDate);
    } else {
        throw std::range_error("Unknown binary exercise type " + excType);
    }

    auto stochProcess = qlext::make_shared<QuantLib::BlackScholesMertonProcess>(QuantLib::Handle<QuantLib::Quote>(spot),
                                                                                QuantLib::Handle<QuantLib::YieldTermStructure>(qTS),
                                                                                QuantLib::Handle<QuantLib::YieldTermStructure>(rTS),
                                                                                QuantLib::Handle<QuantLib::BlackVolTermStructure>(volTS));

    QuantLib::ext::shared_ptr<QuantLib::PricingEngine> engine;
    if (excType=="american") {
        engine = qlext::make_shared<QuantLib::AnalyticDigitalAmericanEngine>(stochProcess);
    } else if (excType=="european") {
        engine = qlext::make_shared<QuantLib::AnalyticEuropeanEngine>(stochProcess);
    } else {
        throw std::range_error("Unknown binary exercise type " + excType);
    }

    QuantLib::VanillaOption opt(payoff, exercise);
    opt.setPricingEngine(engine);

    Rcpp::List rl = Rcpp::List::create(Rcpp::Named("value") = opt.NPV(),
                                       Rcpp::Named("delta") = opt.delta(),
                                       Rcpp::Named("gamma") = opt.gamma(),
                                       Rcpp::Named("vega") = (excType=="european") ? opt.vega() : R_NaN,
                                       Rcpp::Named("theta") = (excType=="european") ? opt.theta() : R_NaN,
                                       Rcpp::Named("rho") = (excType=="european") ? opt.rho() : R_NaN,
                                       Rcpp::Named("divRho") = (excType=="european") ? opt.dividendRho() : R_NaN);
    return rl;
}

// dumped core when we tried last
// no longer under 0.3.10 and g++ 4.0.1 (Aug 2005)
// [[Rcpp::export]]
double binaryOptionImpliedVolatilityEngine(std::string type,
                                           double value,
                                           double underlying,
                                           double strike,
                                           double dividendYield,
                                           double riskFreeRate,
                                           double maturity,
                                           double volatility,
                                           double cashPayoff) {

    QuantLib::Option::Type optionType = getOptionType(type);

    // updated again for QuantLib 0.9.0,
    // cf QuantLib-0.9.0/test-suite/digitaloption.cpp
    QuantLib::Date today = QuantLib::Date::todaysDate();
    QuantLib::Settings::instance().evaluationDate() = today;
    QuantLib::DayCounter dc = QuantLib::Actual360();
    QuantLib::Date exDate = getFutureDate(today, maturity);
    auto spot  = qlext::make_shared<QuantLib::SimpleQuote>(underlying);
    auto qRate = qlext::make_shared<QuantLib::SimpleQuote>(dividendYield);
    auto qTS   = flatRate(today, qRate, dc);
    auto rRate = qlext::make_shared<QuantLib::SimpleQuote>(riskFreeRate);
    auto rTS   = flatRate(today, rRate, dc);
    auto vol   = qlext::make_shared<QuantLib::SimpleQuote>(volatility);
    auto volTS = flatVol(today, vol, dc);

    auto payoff = qlext::make_shared<QuantLib::CashOrNothingPayoff>(optionType, strike, cashPayoff);

    auto exercise = qlext::make_shared<QuantLib::EuropeanExercise>(exDate);

    auto stochProcess = qlext::make_shared<QuantLib::BlackScholesMertonProcess>(QuantLib::Handle<QuantLib::Quote>(spot),
                                                                                QuantLib::Handle<QuantLib::YieldTermStructure>(qTS),
                                                                                QuantLib::Handle<QuantLib::YieldTermStructure>(rTS),
                                                                                QuantLib::Handle<QuantLib::BlackVolTermStructure>(volTS));
    auto engine = qlext::make_shared<QuantLib::AnalyticBarrierEngine>(stochProcess);

    QuantLib::VanillaOption opt(payoff, exercise);
    opt.setPricingEngine(engine);

    return opt.impliedVolatility(value, stochProcess);
}

// [[Rcpp::export]]
Rcpp::List barrierOptionEngine(std::string barrType,
                               std::string type,
                               double underlying,
                               double strike,
                               double dividendYield,
                               double riskFreeRate,
                               double maturity,
                               double volatility,
                               double barrier,
                               double rebate) {

    QuantLib::Barrier::Type barrierType = QuantLib::Barrier::DownIn;
    if (barrType=="downin") {
        barrierType = QuantLib::Barrier::DownIn;
    } else if (barrType=="upin") {
        barrierType = QuantLib::Barrier::UpIn;
    } else if (barrType=="downout") {
        barrierType = QuantLib::Barrier::DownOut;
    } else if (barrType=="upout") {
        barrierType = QuantLib::Barrier::UpOut;
    } else {
        throw std::range_error("Unknown barrier type " + type);
    }

    QuantLib::Option::Type optionType = getOptionType(type);

    // new QuantLib 0.3.5 framework, updated for 0.3.7
    // updated again for QuantLib 0.9.0,
    // cf QuantLib-0.9.0/test-suite/barrieroption.cpp
    QuantLib::Date today = QuantLib::Date::todaysDate();
    QuantLib::Settings::instance().evaluationDate() = today;
    QuantLib::DayCounter dc = QuantLib::Actual360();
    QuantLib::Date exDate = getFutureDate(today, maturity);
    auto spot = qlext::make_shared<QuantLib::SimpleQuote>(underlying);
    auto qRate = qlext::make_shared<QuantLib::SimpleQuote>(dividendYield);
    auto qTS = flatRate(today, qRate, dc);
    auto rRate = qlext::make_shared<QuantLib::SimpleQuote>(riskFreeRate);
    auto rTS = flatRate(today,rRate,dc);
    auto vol = qlext::make_shared<QuantLib::SimpleQuote>(volatility);
    auto volTS = flatVol(today, vol, dc);

    auto exercise = qlext::make_shared<QuantLib::EuropeanExercise>(exDate);

    auto payoff = qlext::make_shared<QuantLib::PlainVanillaPayoff>(optionType, strike);

    auto stochProcess = qlext::make_shared<QuantLib::BlackScholesMertonProcess>(QuantLib::Handle<QuantLib::Quote>(spot),
                                                                                QuantLib::Handle<QuantLib::YieldTermStructure>(qTS),
                                                                                QuantLib::Handle<QuantLib::YieldTermStructure>(rTS),
                                                                                QuantLib::Handle<QuantLib::BlackVolTermStructure>(volTS));

    // Size timeSteps = 1;
    // bool antitheticVariate = false;
    // bool controlVariate = false;
    // Size requiredSamples = 10000;
    // double requiredTolerance = 0.02;
    // Size maxSamples = 1000000;
    // bool isBiased = false;

    auto engine = qlext::make_shared<QuantLib::AnalyticBarrierEngine>(stochProcess);

    // need to explicitly reference BarrierOption from QuantLib here
    QuantLib::BarrierOption barrierOption(barrierType,
                                          barrier,
                                          rebate,
                                          payoff,
                                          exercise);
    barrierOption.setPricingEngine(engine);

    Rcpp::List rl = Rcpp::List::create(Rcpp::Named("value") = barrierOption.NPV(),
                                       Rcpp::Named("delta") = R_NaReal,
                                       Rcpp::Named("gamma") = R_NaReal,
                                       Rcpp::Named("vega") = R_NaReal,
                                       Rcpp::Named("theta") = R_NaReal,
                                       Rcpp::Named("rho") = R_NaReal,
                                       Rcpp::Named("divRho") = R_NaReal);
    return rl;
}
