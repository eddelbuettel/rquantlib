
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2002 - 2024  Dirk Eddelbuettel
//  Copyright (C) 2009 - 2010  Dirk Eddelbuettel and Khanh Nguyen
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
Rcpp::List asianOptionEngine(std::string averageType,
                             std::string type,
                             double underlying,
                             double strike,
                             double dividendYield,
                             double riskFreeRate,
                             double maturity,
                             double volatility,
                             double first,
                             double length,
                             size_t fixings) {

    QuantLib::Option::Type optionType = getOptionType(type);

    //from test-suite/asionoptions.cpp
    QuantLib::DayCounter dc = QuantLib::Actual360();
    QuantLib::Date today = QuantLib::Date::todaysDate();
    QuantLib::Settings::instance().evaluationDate() = today;

    auto spot = qlext::make_shared<QuantLib::SimpleQuote>(underlying);
    auto qRate = qlext::make_shared<QuantLib::SimpleQuote>(dividendYield);
    auto qTS = flatRate(today, qRate, dc);
    auto rRate = qlext::make_shared<QuantLib::SimpleQuote>(riskFreeRate);
    auto rTS  = flatRate(today, rRate, dc);
    auto vol = qlext::make_shared<QuantLib::SimpleQuote>(volatility);
    auto volTS = flatVol(today, vol, dc);

    typedef QuantLib::BlackScholesMertonProcess qlBSMp;
    auto stochProcess = qlext::make_shared<qlBSMp>(QuantLib::Handle<QuantLib::Quote>(spot),
                                                   QuantLib::Handle<QuantLib::YieldTermStructure>(qTS),
                                                   QuantLib::Handle<QuantLib::YieldTermStructure>(rTS),
                                                   QuantLib::Handle<QuantLib::BlackVolTermStructure>(volTS));


    auto payoff = qlext::make_shared<QuantLib::PlainVanillaPayoff>(optionType,strike);

    Rcpp::List rl = R_NilValue;

    if (averageType=="geometric"){
        auto engine = qlext::make_shared<QuantLib::AnalyticContinuousGeometricAveragePriceAsianEngine>(stochProcess);

        QuantLib::Date exDate = getFutureDate(today, maturity);

        auto exercise = qlext::make_shared<QuantLib::EuropeanExercise>(exDate);
        QuantLib::ContinuousAveragingAsianOption option(QuantLib::Average::Geometric, payoff, exercise);
        option.setPricingEngine(engine);

        rl = Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                Rcpp::Named("delta") = option.delta(),
                                Rcpp::Named("gamma") = option.gamma(),
                                Rcpp::Named("vega") = option.vega(),
                                Rcpp::Named("theta") = option.theta(),
                                Rcpp::Named("rho") = option.rho(),
                                Rcpp::Named("divRho") = option.dividendRho());

    } else if (averageType=="arithmetic") {

        // TODO:  check fixings > 1, first, length
        if (first < 0) Rcpp::stop("Parameter 'first' must be non-negative.");
        if (length < 0) Rcpp::stop("Parameter 'length' must be non-negative.");
        if (fixings <= 1) Rcpp::stop("Parameter 'fixings' must be larger than one.");

        auto engine =
            QuantLib::MakeMCDiscreteArithmeticAPEngine<QuantLib::LowDiscrepancy>(stochProcess)
            .withSamples(2047)
            .withControlVariate();

        //QuantLib::ext::shared_ptr<PricingEngine> engine =
        //    MakeMCDiscreteArithmeticASEngine<LowDiscrepancy>(stochProcess)
        //    .withSeed(3456789)
        //    .withSamples(1023);

        QuantLib::Time dt = length / (fixings - 1);

        std::vector<QuantLib::Time> timeIncrements(fixings);
        std::vector<QuantLib::Date> fixingDates(fixings);
        timeIncrements[0] = first;
        fixingDates[0] = today + QuantLib::Integer(timeIncrements[0] * 360 + 0.5);
        for (QuantLib::Size i=1; i<fixings; i++) {
            timeIncrements[i] = i*dt + first;
            fixingDates[i] = getFutureDate(today, timeIncrements[i]);
        }
        QuantLib::Real runningSum = 0.0;
        QuantLib::Size pastFixing = 0;

        auto exercise = qlext::make_shared<QuantLib::EuropeanExercise>(fixingDates[fixings-1]);

        QuantLib::DiscreteAveragingAsianOption option(QuantLib::Average::Arithmetic,
                                                      runningSum,
                                                      pastFixing,
                                                      fixingDates,
                                                      payoff,
                                                      exercise);
        option.setPricingEngine(engine);
        rl = Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                Rcpp::Named("delta") = R_NaReal,
                                Rcpp::Named("gamma") = R_NaReal,
                                Rcpp::Named("vega") = R_NaReal,
                                Rcpp::Named("theta") = R_NaReal,
                                Rcpp::Named("rho") = R_NaReal,
                                Rcpp::Named("divRho") = R_NaReal);
    }
    return rl;
}
