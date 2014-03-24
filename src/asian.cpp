// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
//
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2002 - 2014  Dirk Eddelbuettel 
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

#include <rquantlib.h>

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

    boost::shared_ptr<QuantLib::SimpleQuote> spot(new QuantLib::SimpleQuote(underlying));
    boost::shared_ptr<QuantLib::SimpleQuote> qRate(new QuantLib::SimpleQuote(dividendYield));
    boost::shared_ptr<QuantLib::YieldTermStructure> qTS = flatRate(today, qRate, dc);
    boost::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote(riskFreeRate));
    boost::shared_ptr<QuantLib::YieldTermStructure> rTS = flatRate(today, rRate, dc);
    boost::shared_ptr<QuantLib::SimpleQuote> vol(new QuantLib::SimpleQuote(volatility));
    boost::shared_ptr<QuantLib::BlackVolTermStructure> volTS = flatVol(today, vol, dc);
        
    boost::shared_ptr<QuantLib::BlackScholesMertonProcess>
        stochProcess(new
                     QuantLib::BlackScholesMertonProcess(QuantLib::Handle<QuantLib::Quote>(spot),
                                                         QuantLib::Handle<QuantLib::YieldTermStructure>(qTS),
                                                         QuantLib::Handle<QuantLib::YieldTermStructure>(rTS),
                                                         QuantLib::Handle<QuantLib::BlackVolTermStructure>(volTS)));

    boost::shared_ptr<QuantLib::StrikedTypePayoff> 
        payoff(new QuantLib::PlainVanillaPayoff(optionType,strike));

    Rcpp::List rl = R_NilValue;
   
    if (averageType=="geometric"){
        boost::shared_ptr<QuantLib::PricingEngine> 
            engine(new
                   QuantLib::AnalyticContinuousGeometricAveragePriceAsianEngine(stochProcess));
            
        QuantLib::Date exDate = today + int(maturity * 360 + 0.5);
        boost::shared_ptr<QuantLib::Exercise> exercise(new QuantLib::EuropeanExercise(exDate));
        QuantLib::ContinuousAveragingAsianOption option(QuantLib::Average::Geometric,
                                                        payoff, exercise);
        option.setPricingEngine(engine);
            
        rl = Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                Rcpp::Named("delta") = option.delta(),
                                Rcpp::Named("gamma") = option.gamma(),
                                Rcpp::Named("vega") = option.vega(),
                                Rcpp::Named("theta") = option.theta(),
                                Rcpp::Named("rho") = option.rho(),
                                Rcpp::Named("divRho") = option.dividendRho());
            
    } else if (averageType=="arithmetic"){
        boost::shared_ptr<QuantLib::PricingEngine> engine =
            QuantLib::MakeMCDiscreteArithmeticAPEngine<QuantLib::LowDiscrepancy>(stochProcess)
            .withSamples(2047)
            .withControlVariate();
            
        //boost::shared_ptr<PricingEngine> engine =
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
            fixingDates[i] = today + QuantLib::Integer(timeIncrements[i]*360+0.5);
        }
        QuantLib::Real runningSum = 0.0;
        QuantLib::Size pastFixing = 0;

        boost::shared_ptr<QuantLib::Exercise> 
            exercise(new QuantLib::EuropeanExercise(fixingDates[fixings-1]));

        QuantLib::DiscreteAveragingAsianOption option(QuantLib::Average::Arithmetic, 
                                                      runningSum,
                                                      pastFixing, 
                                                      fixingDates,
                                                      payoff, 
                                                      exercise);
        option.setPricingEngine(engine);
        rl = Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                Rcpp::Named("delta") = R_NaN,
                                Rcpp::Named("gamma") = R_NaN,
                                Rcpp::Named("vega") = R_NaN,
                                Rcpp::Named("theta") = R_NaN,
                                Rcpp::Named("rho") = R_NaN,
                                Rcpp::Named("divRho") = R_NaN);
    }
    return rl;
}

