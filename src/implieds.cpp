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

#include "rquantlib.h"

RcppExport  SEXP EuropeanOptionImpliedVolatility(SEXP optionParameters) {
    const QuantLib::Size maxEvaluations = 100;
    const double tolerance = 1.0e-6;
  
    try {

        Rcpp::List rparam(optionParameters); 

        std::string type = Rcpp::as<std::string>(rparam["type"]);
        double value = Rcpp::as<double>(rparam["value"]);
        double underlying = Rcpp::as<double>(rparam["underlying"]);
        double strike = Rcpp::as<double>(rparam["strike"]);
        QuantLib::Spread dividendYield = Rcpp::as<double>(rparam["dividendYield"]);
        QuantLib::Rate riskFreeRate = Rcpp::as<double>(rparam["riskFreeRate"]);
        QuantLib::Time maturity = Rcpp::as<double>(rparam["maturity"]);
        int length = int(maturity*360 + 0.5); // FIXME: this could be better
        double volatility = Rcpp::as<double>(rparam["volatility"]);

        QuantLib::Option::Type optionType = getOptionType(type);

        QuantLib::Date today = QuantLib::Date::todaysDate();
        QuantLib::Settings::instance().evaluationDate() = today;

        // new framework as per QuantLib 0.3.5
        // updated for 0.3.7
        QuantLib::DayCounter dc = QuantLib::Actual360();

        boost::shared_ptr<QuantLib::SimpleQuote> spot(new QuantLib::SimpleQuote(underlying));
        boost::shared_ptr<QuantLib::SimpleQuote> vol(new QuantLib::SimpleQuote(volatility));
        boost::shared_ptr<QuantLib::BlackVolTermStructure> volTS = flatVol(today, vol, dc);
        boost::shared_ptr<QuantLib::SimpleQuote> qRate(new QuantLib::SimpleQuote(dividendYield));
        boost::shared_ptr<QuantLib::YieldTermStructure> qTS = flatRate(today,qRate,dc);
        boost::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote(riskFreeRate));
        boost::shared_ptr<QuantLib::YieldTermStructure> rTS = flatRate(today,rRate,dc);
        QuantLib::Date exDate = today + length;
        boost::shared_ptr<QuantLib::Exercise> exercise(new QuantLib::EuropeanExercise(exDate));
        boost::shared_ptr<QuantLib::StrikedTypePayoff> payoff(new QuantLib::PlainVanillaPayoff(optionType, strike));
        double implVol = 0.0; // just to remove a warning...
        boost::shared_ptr<QuantLib::VanillaOption> option = 
            makeOption(payoff, exercise, spot, qTS, rTS, volTS, 
                       Analytic, QuantLib::Null<QuantLib::Size>(), 
                       QuantLib::Null<QuantLib::Size>());

        boost::shared_ptr<QuantLib::GeneralizedBlackScholesProcess> process = makeProcess(spot, qTS, rTS,volTS);

        double volguess = volatility;
        vol->setValue(volguess);

        implVol = option->impliedVolatility(value, process, tolerance, maxEvaluations);

        return Rcpp::List::create(Rcpp::Named("impliedVol") = implVol,
                                  Rcpp::Named("parameters") = optionParameters);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport  SEXP AmericanOptionImpliedVolatility(SEXP optionParameters) {
    const QuantLib::Size maxEvaluations = 100;
    const double tolerance = 1.0e-6;
  
    try {

        Rcpp::List rparam(optionParameters);

        std::string type = Rcpp::as<std::string>(rparam["type"]);
        double value = Rcpp::as<double>(rparam["value"]);
        double underlying = Rcpp::as<double>(rparam["underlying"]);
        double strike = Rcpp::as<double>(rparam["strike"]);
        QuantLib::Spread dividendYield = Rcpp::as<double>(rparam["dividendYield"]);
        QuantLib::Rate riskFreeRate = Rcpp::as<double>(rparam["riskFreeRate"]);
        QuantLib::Time maturity = Rcpp::as<double>(rparam["maturity"]);
        int length = int(maturity*360 + 0.5); // FIXME: this could be better
        double volguess = Rcpp::as<double>(rparam["volatility"]);

        QuantLib::Option::Type optionType = getOptionType(type);

        QuantLib::Date today = QuantLib::Date::todaysDate();
        QuantLib::Settings::instance().evaluationDate() = today;

        // new framework as per QuantLib 0.3.5
        QuantLib::DayCounter dc = QuantLib::Actual360();
        boost::shared_ptr<QuantLib::SimpleQuote> spot(new QuantLib::SimpleQuote(underlying));
        boost::shared_ptr<QuantLib::SimpleQuote> vol(new QuantLib::SimpleQuote(volguess));
        boost::shared_ptr<QuantLib::BlackVolTermStructure> volTS = flatVol(today, vol,dc);
        boost::shared_ptr<QuantLib::SimpleQuote> qRate(new QuantLib::SimpleQuote(dividendYield));
        boost::shared_ptr<QuantLib::YieldTermStructure> qTS = flatRate(today,qRate,dc);
        boost::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote(riskFreeRate));
        boost::shared_ptr<QuantLib::YieldTermStructure> rTS = flatRate(today,rRate,dc);

        QuantLib::Date exDate = today + length;
        QuantLib::Settings::instance().evaluationDate() = today;

        //boost::shared_ptr<Exercise> exercise(new EuropeanExercise(exDate));
        boost::shared_ptr<QuantLib::Exercise> exercise(new QuantLib::AmericanExercise(today, exDate));
        boost::shared_ptr<QuantLib::StrikedTypePayoff> payoff(new QuantLib::PlainVanillaPayoff(optionType, strike));
        boost::shared_ptr<QuantLib::VanillaOption> option = makeOption(payoff, exercise, spot, qTS, rTS, volTS, JR);

        boost::shared_ptr<QuantLib::GeneralizedBlackScholesProcess> process = makeProcess(spot, qTS, rTS,volTS);

        double implVol = 0.0; // just to remove a warning...
        implVol = option->impliedVolatility(value, process, tolerance, maxEvaluations);

        return Rcpp::List::create(Rcpp::Named("impliedVol") = implVol,
                                  Rcpp::Named("parameters") = optionParameters);

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}
 
