// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2002 - 2010  Dirk Eddelbuettel <edd@debian.org>
//
// $Id$
//
// This file is part of the RQuantLib library for GNU R.
// It is made available under the terms of the GNU General Public
// License, version 2, or at your option, any later version,
// incorporated herein by reference.
//
// This program is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free
// Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA

#include "rquantlib.h"

RcppExport  SEXP QL_BinaryOption(SEXP optionParameters) {

    try {
        Rcpp::List rparam(optionParameters);

        std::string binType = Rcpp::as<std::string>(rparam["binType"]);
        std::string type = Rcpp::as<std::string>(rparam["type"]);
        std::string excType = Rcpp::as<std::string>(rparam["excType"]);
        double underlying = Rcpp::as<double>(rparam["underlying"]);
        double strike = Rcpp::as<double>(rparam["strike"]);
        Spread dividendYield = Rcpp::as<double>(rparam["dividendYield"]);
        Rate riskFreeRate = Rcpp::as<double>(rparam["riskFreeRate"]);
        Time maturity = Rcpp::as<double>(rparam["maturity"]);
        int length = int(maturity*360 + 0.5); // FIXME: this could be better, but same rounding in QL
        double volatility = Rcpp::as<double>(rparam["volatility"]);
        double cashPayoff = Rcpp::as<double>(rparam["cashPayoff"]);
    
        Option::Type optionType = getOptionType(type);

        // new QuantLib 0.3.5 framework: digitals, updated for 0.3.7
        // updated again for QuantLib 0.9.0, 
        // cf QuantLib-0.9.0/test-suite/digitaloption.cpp
        Date today = Date::todaysDate();
        Settings::instance().evaluationDate() = today;

        DayCounter dc = Actual360();
        boost::shared_ptr<SimpleQuote> spot(new SimpleQuote(underlying));
        boost::shared_ptr<SimpleQuote> qRate(new SimpleQuote(dividendYield));
        boost::shared_ptr<YieldTermStructure> qTS = flatRate(today,qRate,dc);
        boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(riskFreeRate));
        boost::shared_ptr<YieldTermStructure> rTS = flatRate(today,rRate,dc);
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote(volatility));
        boost::shared_ptr<BlackVolTermStructure> volTS = flatVol(today, vol, dc);

        boost::shared_ptr<StrikedTypePayoff> payoff;
        if (binType=="cash") {
            boost::shared_ptr<StrikedTypePayoff> con(new CashOrNothingPayoff(optionType, strike, cashPayoff));
            payoff = con;
        } else if (binType=="asset") {
            boost::shared_ptr<StrikedTypePayoff> aon(new AssetOrNothingPayoff(optionType, strike));
            payoff = aon;
        } else if (binType=="gap") {
            boost::shared_ptr<StrikedTypePayoff> gap(new GapPayoff(optionType, strike, cashPayoff));
            payoff = gap;
        } else {
            throw std::range_error("Unknown binary option type " + binType);
        }

        Date exDate = today + length;
        boost::shared_ptr<Exercise> exercise;
        if (excType=="american") {
            boost::shared_ptr<Exercise> amEx(new AmericanExercise(today, exDate));
            exercise = amEx;
        } else if (excType=="european") {
            boost::shared_ptr<Exercise> euEx(new EuropeanExercise(exDate));
            exercise = euEx;
        } else {
            throw std::range_error("Unknown binary exercise type " + excType);
        }

        boost::shared_ptr<BlackScholesMertonProcess> stochProcess(new 
              BlackScholesMertonProcess(Handle<Quote>(spot),
                                        Handle<YieldTermStructure>(qTS),
                                        Handle<YieldTermStructure>(rTS),
                                        Handle<BlackVolTermStructure>(volTS)));

        boost::shared_ptr<PricingEngine> engine;
        if (excType=="american") {
            boost::shared_ptr<PricingEngine> amEng(new AnalyticDigitalAmericanEngine(stochProcess));
            engine = amEng;
        } else if (excType=="european") {
            boost::shared_ptr<PricingEngine> euEng(new AnalyticEuropeanEngine(stochProcess));
            engine = euEng;
        } else {
            throw std::range_error("Unknown binary exercise type " + excType);
        }

        VanillaOption opt(payoff, exercise);
        opt.setPricingEngine(engine);

        Rcpp::List rl = Rcpp::List::create(Rcpp::Named("value") = opt.NPV(),
                                           Rcpp::Named("delta") = opt.delta(),
                                           Rcpp::Named("gamma") = opt.gamma(),
                                           Rcpp::Named("vega") = (excType=="european") ? opt.vega() : R_NaN,
                                           Rcpp::Named("theta") = (excType=="european") ? opt.theta() : R_NaN,
                                           Rcpp::Named("rho") = (excType=="european") ? opt.rho() : R_NaN,
                                           Rcpp::Named("divRho") = (excType=="european") ? opt.dividendRho() : R_NaN,
                                           Rcpp::Named("parameters") = optionParameters);
        return rl;

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

// dumped core when we tried last
// no longer under 0.3.10 and g++ 4.0.1 (Aug 2005)
RcppExport  SEXP QL_BinaryOptionImpliedVolatility(SEXP optionParameters) {

    try {

        Rcpp::List rparam(optionParameters);

        std::string type = Rcpp::as<std::string>(rparam["type"]);
        double value = Rcpp::as<double>(rparam["value"]);
        double underlying = Rcpp::as<double>(rparam["underlying"]);
        double strike = Rcpp::as<double>(rparam["strike"]);
        Spread dividendYield = Rcpp::as<double>(rparam["dividendYield"]);
        Rate riskFreeRate = Rcpp::as<double>(rparam["riskFreeRate"]);
        Time maturity = Rcpp::as<double>(rparam["maturity"]);
        int length = int(maturity*360 + 0.5); // FIXME: this could be better
        double volatility = Rcpp::as<double>(rparam["volatility"]);
        double cashPayoff = Rcpp::as<double>(rparam["cashPayoff"]);

        Option::Type optionType = getOptionType(type);

        // updated again for QuantLib 0.9.0, 
        // cf QuantLib-0.9.0/test-suite/digitaloption.cpp
        Date today = Date::todaysDate();
        Settings::instance().evaluationDate() = today;
        DayCounter dc = Actual360();
        boost::shared_ptr<SimpleQuote> spot(new SimpleQuote(underlying));
        boost::shared_ptr<SimpleQuote> qRate(new SimpleQuote(dividendYield));
        boost::shared_ptr<YieldTermStructure> qTS = flatRate(today, qRate, dc);
        boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(riskFreeRate));
        boost::shared_ptr<YieldTermStructure> rTS = flatRate(today, rRate, dc);
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote(volatility));
        boost::shared_ptr<BlackVolTermStructure> volTS = flatVol(today, vol, dc);

        boost::shared_ptr<StrikedTypePayoff> 
            payoff(new CashOrNothingPayoff(optionType, strike, cashPayoff));

        Date exDate = today + length;
        boost::shared_ptr<Exercise> exercise(new EuropeanExercise(exDate));

        boost::shared_ptr<BlackScholesMertonProcess> stochProcess(new 
               BlackScholesMertonProcess(Handle<Quote>(spot),
                                         Handle<YieldTermStructure>(qTS),
                                         Handle<YieldTermStructure>(rTS),
                                         Handle<BlackVolTermStructure>(volTS)));
        //boost::shared_ptr<PricingEngine> engine(new AnalyticEuropeanEngine(stochProcess));
        boost::shared_ptr<PricingEngine> engine(new AnalyticBarrierEngine(stochProcess));

        VanillaOption opt(payoff, exercise);
        opt.setPricingEngine(engine);

        return Rcpp::List::create(Rcpp::Named("impliedVol") = opt.impliedVolatility(value, stochProcess),
                                  Rcpp::Named("parameters") = optionParameters);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport  SEXP QL_BarrierOption(SEXP optionParameters) {

    try {

        Rcpp::List rparam(optionParameters);   

        std::string barrType = Rcpp::as<std::string>(rparam["barrType"]);
        std::string type = Rcpp::as<std::string>(rparam["type"]);
        double underlying = Rcpp::as<double>(rparam["underlying"]);
        double strike = Rcpp::as<double>(rparam["strike"]);
        Spread dividendYield = Rcpp::as<double>(rparam["dividendYield"]);
        Rate riskFreeRate = Rcpp::as<double>(rparam["riskFreeRate"]);
        Time maturity = Rcpp::as<double>(rparam["maturity"]);
        int length = int(maturity*360 + 0.5); // FIXME: this could be better
        double volatility = Rcpp::as<double>(rparam["volatility"]);
        double barrier = Rcpp::as<double>(rparam["barrier"]);
        double rebate = Rcpp::as<double>(rparam["rebate"]);
        
        Barrier::Type barrierType=Barrier::DownIn;
        if (barrType=="downin") {
            barrierType = Barrier::DownIn;
        } else if (barrType=="upin") {
            barrierType = Barrier::UpIn;
        } else if (barrType=="downout") {
            barrierType = Barrier::DownOut;
        } else if (barrType=="upout") {
            barrierType = Barrier::UpOut;
        } else {
            throw std::range_error("Unknown barrier type " + type);
        }

        Option::Type optionType = getOptionType(type);

        // new QuantLib 0.3.5 framework, updated for 0.3.7
        // updated again for QuantLib 0.9.0, 
        // cf QuantLib-0.9.0/test-suite/barrieroption.cpp
        Date today = Date::todaysDate();
        Settings::instance().evaluationDate() = today;
        DayCounter dc = Actual360();
        boost::shared_ptr<SimpleQuote> spot(new SimpleQuote(underlying));
        boost::shared_ptr<SimpleQuote> qRate(new SimpleQuote(dividendYield));
        boost::shared_ptr<YieldTermStructure> qTS = flatRate(today, qRate, dc);
        boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(riskFreeRate));
        boost::shared_ptr<YieldTermStructure> rTS = flatRate(today,rRate,dc);
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote(volatility));
        boost::shared_ptr<BlackVolTermStructure> volTS = flatVol(today, vol, dc);

        Date exDate = today + length;
        boost::shared_ptr<Exercise> exercise(new EuropeanExercise(exDate));
        
        boost::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(optionType, strike));

        boost::shared_ptr<BlackScholesMertonProcess> 
            stochProcess(new BlackScholesMertonProcess(
                	Handle<Quote>(spot),
                	Handle<YieldTermStructure>(qTS),
                	Handle<YieldTermStructure>(rTS),
                	Handle<BlackVolTermStructure>(volTS)));

        // Size timeSteps = 1;
        // bool antitheticVariate = false;
        // bool controlVariate = false;
        // Size requiredSamples = 10000;
        // double requiredTolerance = 0.02;
        // Size maxSamples = 1000000;
        // bool isBiased = false;

        boost::shared_ptr<PricingEngine> engine(new AnalyticBarrierEngine(stochProcess));

        BarrierOption barrierOption(barrierType,
                                    barrier,
                                    rebate,
                                    payoff,
                                    exercise);
        barrierOption.setPricingEngine(engine);

        Rcpp::List rl = Rcpp::List::create(Rcpp::Named("value") = barrierOption.NPV(),
                                           Rcpp::Named("delta") = R_NaN,
                                           Rcpp::Named("gamma") = R_NaN,
                                           Rcpp::Named("vega") = R_NaN,
                                           Rcpp::Named("theta") = R_NaN,
                                           Rcpp::Named("rho") = R_NaN,
                                           Rcpp::Named("divRho") = R_NaN,
                                           Rcpp::Named("parameters") = optionParameters);
        return rl;

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

