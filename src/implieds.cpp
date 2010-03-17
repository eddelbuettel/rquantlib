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

// NB can be build standalone as   PKG_LIBS=-lQuantLib R CMD SHLIB implieds.cc

#include "rquantlib.hpp"

RcppExport  SEXP QL_EuropeanOptionImpliedVolatility(SEXP optionParameters) {
    const Size maxEvaluations = 100;
    const double tolerance = 1.0e-6;

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

        Option::Type optionType=Option::Call;
        if (type=="call") {
            optionType = Option::Call;
        } else if (type=="put") {
            optionType = Option::Put;
        } else {
            throw std::range_error("Unknown option " + type);
        }

        Date today = Date::todaysDate();

        // new framework as per QuantLib 0.3.5
        // updated for 0.3.7
        DayCounter dc = Actual360();

        boost::shared_ptr<SimpleQuote> spot(new SimpleQuote(underlying));
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote(volatility));
        boost::shared_ptr<BlackVolTermStructure> volTS = flatVol(today, vol, dc);
        boost::shared_ptr<SimpleQuote> qRate(new SimpleQuote(dividendYield));
        boost::shared_ptr<YieldTermStructure> qTS = flatRate(today,qRate,dc);
        boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(riskFreeRate));
        boost::shared_ptr<YieldTermStructure> rTS = flatRate(today,rRate,dc);
        Date exDate = today + length;
        boost::shared_ptr<Exercise> exercise(new EuropeanExercise(exDate));
        boost::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(optionType, strike));
        double implVol = 0.0; // just to remove a warning...
        boost::shared_ptr<VanillaOption> option = 
            makeOption(payoff, exercise, spot, qTS, rTS, volTS, 
                       Analytic, Null<Size>(), Null<Size>());

        boost::shared_ptr<GeneralizedBlackScholesProcess> process =
              makeProcess(spot, qTS, rTS,volTS);

        double volguess = volatility;
        vol->setValue(volguess);

        implVol = option->impliedVolatility(value, process, tolerance, maxEvaluations);

        RcppResultSet rs;
        rs.add("impliedVol", implVol);
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

RcppExport  SEXP QL_AmericanOptionImpliedVolatility(SEXP optionParameters) {
    const Size maxEvaluations = 100;
    const double tolerance = 1.0e-6;

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
        double volguess = rparam.getDoubleValue("volatility");

        Option::Type optionType=Option::Call;
        if (type=="call") {
            optionType = Option::Call;
        } else if (type=="put") {
            optionType = Option::Put;
        } else {
            throw std::range_error("Unknown option " + type);
        }

        Date today = Date::todaysDate();

        // new framework as per QuantLib 0.3.5
        DayCounter dc = Actual360();
        boost::shared_ptr<SimpleQuote> spot(new SimpleQuote(underlying));
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote(volguess));
        boost::shared_ptr<BlackVolTermStructure> volTS = flatVol(today, vol,dc);
        boost::shared_ptr<SimpleQuote> qRate(new SimpleQuote(dividendYield));
        boost::shared_ptr<YieldTermStructure> qTS = flatRate(today,qRate,dc);
        boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(riskFreeRate));
        boost::shared_ptr<YieldTermStructure> rTS = flatRate(today,rRate,dc);

        Date exDate = today + length;
        //boost::shared_ptr<Exercise> exercise(new EuropeanExercise(exDate));
        boost::shared_ptr<Exercise> exercise(new AmericanExercise(today, exDate));
        boost::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(optionType, strike));
        boost::shared_ptr<VanillaOption> option = makeOption(payoff, exercise, spot, qTS, rTS, volTS, JR);

        boost::shared_ptr<GeneralizedBlackScholesProcess> process = makeProcess(spot, qTS, rTS,volTS);

        double implVol = 0.0; // just to remove a warning...
        implVol = option->impliedVolatility(value, process, tolerance, maxEvaluations);

        RcppResultSet rs;
        rs.add("impliedVol", implVol);
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
 
