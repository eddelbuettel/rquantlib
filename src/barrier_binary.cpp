// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2002 - 2009 Dirk Eddelbuettel <edd@debian.org>
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

// NB can be build standalone as   PKG_LIBS=-lQuantLib R CMD SHLIB RQuantLib.cc

#include "rquantlib.hpp"

RcppExport  SEXP QL_BinaryOption(SEXP optionParameters) {

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
  
    try {

        RcppParams rparam(optionParameters);    	// Parameter wrapper class

        std::string binType = rparam.getStringValue("binType");
        std::string type = rparam.getStringValue("type");
        std::string excType = rparam.getStringValue("excType");
        double underlying = rparam.getDoubleValue("underlying");
        double strike = rparam.getDoubleValue("strike");
        Spread dividendYield = rparam.getDoubleValue("dividendYield");
        Rate riskFreeRate = rparam.getDoubleValue("riskFreeRate");
        Time maturity = rparam.getDoubleValue("maturity");
        int length = int(maturity*360 + 0.5); // FIXME: this could be better, but same rounding in QL
        double volatility = rparam.getDoubleValue("volatility");
        double cashPayoff = rparam.getDoubleValue("cashPayoff");
    
        Option::Type optionType=Option::Call;
        if (type=="call") {
            optionType = Option::Call;
        } else if (type=="put") {
            optionType = Option::Put;
        } else {
            throw std::range_error("Unknown option " + type);
        }

        // new QuantLib 0.3.5 framework: digitals, updated for 0.3.7
        // updated again for QuantLib 0.9.0, 
        // cf QuantLib-0.9.0/test-suite/digitaloption.cpp
        Date today = Date::todaysDate();
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

        RcppResultSet rs;
        rs.add("value", opt.NPV());
        rs.add("delta", opt.delta());
        rs.add("gamma", opt.gamma());
        if (excType=="european") {
            rs.add("vega", opt.vega());
            rs.add("theta", opt.theta());
            rs.add("rho", opt.rho());
            rs.add("divRho", opt.dividendRho());
        } else {
            rs.add("vega", R_NaN);
            rs.add("theta", R_NaN);
            rs.add("rho", R_NaN);
            rs.add("divRho", R_NaN);
        }
        rs.add("parameters", optionParameters, false);
        rl = rs.getReturnList();

    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
  
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}

// dumped core when we tried last
// no longer under 0.3.10 and g++ 4.0.1 (Aug 2005)
RcppExport  SEXP QL_BinaryOptionImpliedVolatility(SEXP optionParameters) {

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
  
    try {

        RcppParams rparam(optionParameters);    	// Parameter wrapper class

        std::string type = rparam.getStringValue("type");
        double value = rparam.getDoubleValue("value");
        double underlying = rparam.getDoubleValue("underlying");
        double strike = rparam.getDoubleValue("strike");
        Spread dividendYield = rparam.getDoubleValue("dividendYield");
        Rate riskFreeRate = rparam.getDoubleValue("riskFreeRate");
        Time maturity = rparam.getDoubleValue("maturity");
        int length = int(maturity*360 + 0.5); // FIXME: this could be better
        double volatility = rparam.getDoubleValue("volatility");
        double cashPayoff = rparam.getDoubleValue("cashPayoff");

        Option::Type optionType=Option::Call;
        if (type=="call") {
            optionType = Option::Call;
        } else if (type=="put") {
            optionType = Option::Put;
        } else {
            throw std::range_error("Unknown option " + type);
        }

        // updated again for QuantLib 0.9.0, 
        // cf QuantLib-0.9.0/test-suite/digitaloption.cpp
        Date today = Date::todaysDate();
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

        RcppResultSet rs;
        rs.add("impliedVol", opt.impliedVolatility(value, stochProcess));
        rs.add("parameters", optionParameters, false);
        rl = rs.getReturnList();

    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
  
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}

RcppExport  SEXP QL_BarrierOption(SEXP optionParameters) {

    SEXP rl=R_NilValue;
    char* exceptionMesg=NULL;
  
    try {

        RcppParams rparam(optionParameters);    	// Parameter wrapper class

        std::string barrType = rparam.getStringValue("barrType");
        std::string type = rparam.getStringValue("type");
        double underlying = rparam.getDoubleValue("underlying");
        double strike = rparam.getDoubleValue("strike");
        Spread dividendYield = rparam.getDoubleValue("dividendYield");
        Rate riskFreeRate = rparam.getDoubleValue("riskFreeRate");
        Time maturity = rparam.getDoubleValue("maturity");
        int length = int(maturity*360 + 0.5); // FIXME: this could be better
        double volatility = rparam.getDoubleValue("volatility");
        double barrier = rparam.getDoubleValue("barrier");
        double rebate = rparam.getDoubleValue("rebate");
        
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

        Option::Type optionType=Option::Call;
        if (type=="call") {
            optionType = Option::Call;
        } else if (type=="put") {
            optionType = Option::Put;
        } else {
            throw std::range_error("Unknown option " + type);
        }

        // new QuantLib 0.3.5 framework, updated for 0.3.7
        // updated again for QuantLib 0.9.0, 
        // cf QuantLib-0.9.0/test-suite/barrieroption.cpp
        Date today = Date::todaysDate();
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

        RcppResultSet rs;
        rs.add("value", barrierOption.NPV());
        rs.add("delta", R_NaN);
        rs.add("gamma", R_NaN);
        rs.add("vega", R_NaN);
        rs.add("theta", R_NaN);
        rs.add("rho", R_NaN);
        rs.add("divRho", R_NaN);
        rs.add("parameters", optionParameters, false);
        rl = rs.getReturnList();

    } catch(std::exception& ex) {
        exceptionMesg = copyMessageToR(ex.what());
    } catch(...) {
        exceptionMesg = copyMessageToR("unknown reason");
    }
  
    if(exceptionMesg != NULL)
        Rf_error(exceptionMesg);
    
    return rl;
}

