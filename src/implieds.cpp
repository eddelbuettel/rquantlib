
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2002 - 2026  Dirk Eddelbuettel <edd@debian.org>
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
double europeanOptionImpliedVolatilityEngine(std::string type,
                                             double value,
                                             double underlying,
                                             double strike,
                                             double dividendYield,
                                             double riskFreeRate,
                                             double maturity,
                                             double volatility,
                                             int dayCounter) {

    const QuantLib::Size maxEvaluations = 100;
    const double tolerance = 1.0e-6;

    QuantLib::Option::Type optionType = getOptionType(type);

    QuantLib::Date evalDate = QuantLib::Settings::instance().evaluationDate();
    QuantLib::Date exDate = getFutureDate(evalDate, maturity);

    // new framework as per QuantLib 0.3.5
    // updated for 0.3.7
    QuantLib::DayCounter dc = getDayCounter(dayCounter);

    auto spot = qlext::make_shared<QuantLib::SimpleQuote>(underlying);
    auto vol = qlext::make_shared<QuantLib::SimpleQuote>(volatility);
    auto volTS = flatVol(evalDate, vol, dc);
    auto qRate = qlext::make_shared<QuantLib::SimpleQuote>(dividendYield);
    auto qTS = flatRate(evalDate,qRate,dc);
    auto rRate = qlext::make_shared<QuantLib::SimpleQuote>(riskFreeRate);
    auto rTS = flatRate(evalDate,rRate,dc);

    auto exercise = qlext::make_shared<QuantLib::EuropeanExercise>(exDate);
    auto payoff = qlext::make_shared<QuantLib::PlainVanillaPayoff>(optionType, strike);
    auto option = makeOption(payoff, exercise, spot, qTS, rTS, volTS, Analytic,
                             QuantLib::Null<QuantLib::Size>(), QuantLib::Null<QuantLib::Size>());
    auto process = makeProcess(spot, qTS, rTS,volTS);

    double volguess = volatility;
    vol->setValue(volguess);

    return option->impliedVolatility(value, process, tolerance, maxEvaluations);
}

// [[Rcpp::export]]
double europeanOptionImpliedVolatilityEngineByDate(std::string type,
                                                   double value,
                                                   double underlying,
                                                   double strike,
                                                   double dividendYield,
                                                   double riskFreeRate,
                                                   QuantLib::Date exDate,
                                                   double volatility,
                                                   int dayCounter) {

    const QuantLib::Size maxEvaluations = 100;
    const double tolerance = 1.0e-6;

    QuantLib::Option::Type optionType = getOptionType(type);

    QuantLib::Date evalDate = QuantLib::Settings::instance().evaluationDate();

    // new framework as per QuantLib 0.3.5
    // updated for 0.3.7
    QuantLib::DayCounter dc = getDayCounter(dayCounter);

    auto spot = qlext::make_shared<QuantLib::SimpleQuote>(underlying);
    auto vol = qlext::make_shared<QuantLib::SimpleQuote>(volatility);
    auto volTS = flatVol(evalDate, vol, dc);
    auto qRate = qlext::make_shared<QuantLib::SimpleQuote>(dividendYield);
    auto qTS = flatRate(evalDate,qRate,dc);
    auto rRate = qlext::make_shared<QuantLib::SimpleQuote>(riskFreeRate);
    auto rTS = flatRate(evalDate,rRate,dc);

    auto exercise = qlext::make_shared<QuantLib::EuropeanExercise>(exDate);
    auto payoff = qlext::make_shared<QuantLib::PlainVanillaPayoff>(optionType, strike);
    auto option = makeOption(payoff, exercise, spot, qTS, rTS, volTS, Analytic,
                             QuantLib::Null<QuantLib::Size>(), QuantLib::Null<QuantLib::Size>());
    auto process = makeProcess(spot, qTS, rTS,volTS);

    double volguess = volatility;
    vol->setValue(volguess);

    return option->impliedVolatility(value, process, tolerance, maxEvaluations);
}

// [[Rcpp::export]]
double americanOptionImpliedVolatilityEngine(std::string type,
                                             double value,
                                             double underlying,
                                             double strike,
                                             double dividendYield,
                                             double riskFreeRate,
                                             double maturity,
                                             double volguess,
                                             int timesteps,
                                             int gridpoints,
                                             int dayCounter) {

    const QuantLib::Size maxEvaluations = 100;
    const double tolerance = 1.0e-6;

    QuantLib::Option::Type optionType = getOptionType(type);

    QuantLib::Date evalDate = QuantLib::Settings::instance().evaluationDate();
    QuantLib::Date exDate = getFutureDate(evalDate, maturity);

    // new framework as per QuantLib 0.3.5
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    auto spot = qlext::make_shared<QuantLib::SimpleQuote>(underlying);
    auto vol = qlext::make_shared<QuantLib::SimpleQuote>(volguess);
    auto volTS = flatVol(evalDate, vol,dc);
    auto qRate = qlext::make_shared<QuantLib::SimpleQuote>(dividendYield);
    auto qTS = flatRate(evalDate,qRate,dc);
    auto rRate = qlext::make_shared<QuantLib::SimpleQuote>(riskFreeRate);
    auto rTS = flatRate(evalDate,rRate,dc);

    auto exercise = qlext::make_shared<QuantLib::AmericanExercise>(evalDate, exDate);
    auto payoff = qlext::make_shared<QuantLib::PlainVanillaPayoff>(optionType, strike);
    auto option = makeOption(payoff, exercise, spot, qTS, rTS, volTS, JR);

    auto process = makeProcess(spot, qTS, rTS, volTS);

    return option->impliedVolatility(value, process, tolerance, maxEvaluations);
}

// [[Rcpp::export]]
double americanOptionImpliedVolatilityEngineByDate(std::string type,
                                                   double value,
                                                   double underlying,
                                                   double strike,
                                                   double dividendYield,
                                                   double riskFreeRate,
                                                   QuantLib::Date exDate,
                                                   double volguess,
                                                   int timesteps,
                                                   int gridpoints,
                                                   int dayCounter) {

    const QuantLib::Size maxEvaluations = 100;
    const double tolerance = 1.0e-6;

    QuantLib::Option::Type optionType = getOptionType(type);
    QuantLib::Date evalDate = QuantLib::Settings::instance().evaluationDate();

    // new framework as per QuantLib 0.3.5
    QuantLib::DayCounter dc = getDayCounter(dayCounter);
    auto spot = qlext::make_shared<QuantLib::SimpleQuote>(underlying);
    auto vol = qlext::make_shared<QuantLib::SimpleQuote>(volguess);
    auto volTS = flatVol(evalDate, vol, dc);
    auto qRate = qlext::make_shared<QuantLib::SimpleQuote>(dividendYield);
    auto qTS = flatRate(evalDate, qRate, dc);
    auto rRate = qlext::make_shared<QuantLib::SimpleQuote>(riskFreeRate);
    auto rTS = flatRate(evalDate, rRate, dc);

    auto exercise = qlext::make_shared<QuantLib::AmericanExercise>(evalDate, exDate);
    auto payoff = qlext::make_shared<QuantLib::PlainVanillaPayoff>(optionType, strike);
    auto option = makeOption(payoff, exercise, spot, qTS, rTS, volTS, JR);

    auto process = makeProcess(spot, qTS, rTS, volTS);

    return option->impliedVolatility(value, process, tolerance, maxEvaluations);
}
