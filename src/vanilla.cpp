// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
//
//  RQuantLib -- R interface to the QuantLib libraries
//
//  Copyright (C) 2002 - 2015  Dirk Eddelbuettel <edd@debian.org>
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

// [[Rcpp::interfaces(r, cpp)]]

// [[Rcpp::export]]
Rcpp::List europeanOptionEngine(std::string type,
                                double underlying,
                                double strike,
                                double dividendYield,
                                double riskFreeRate,
                                double maturity,
                                double volatility,
                                std::vector<double> discreteDividends,
                                std::vector<double> discreteDividendsTimeUntil) {

#ifdef QL_HIGH_RESOLUTION_DATE    
    // in minutes
    boost::posix_time::time_duration length = boost::posix_time::minutes(maturity * 360 * 24 * 60); 
#else
    int length           = int(maturity*360 + 0.5); // FIXME: this could be better
#endif
    
    QuantLib::Option::Type optionType = getOptionType(type);
    QuantLib::Date today = QuantLib::Date::todaysDate();
    QuantLib::Settings::instance().evaluationDate() = today;

    // new framework as per QuantLib 0.3.5
    QuantLib::DayCounter dc = QuantLib::Actual360();
    boost::shared_ptr<QuantLib::SimpleQuote> spot(new QuantLib::SimpleQuote( underlying ));
    boost::shared_ptr<QuantLib::SimpleQuote> vol(new QuantLib::SimpleQuote( volatility ));
    boost::shared_ptr<QuantLib::BlackVolTermStructure> volTS = flatVol(today, vol, dc);
    boost::shared_ptr<QuantLib::SimpleQuote> qRate(new QuantLib::SimpleQuote( dividendYield ));
    boost::shared_ptr<QuantLib::YieldTermStructure> qTS = flatRate(today, qRate, dc);
    boost::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote( riskFreeRate ));
    boost::shared_ptr<QuantLib::YieldTermStructure> rTS = flatRate(today, rRate, dc);
    
    int n = discreteDividendsTimeUntil.size();
    std::vector<QuantLib::Date> discreteDividendDates;

#ifdef QL_HIGH_RESOLUTION_DATE
    QuantLib::Date exDate(today.dateTime() + length);

    if(discreteDividends[0] != 0) {
        boost::posix_time::time_duration discreteDividendLength;        
        for (int i = 0; i < n; i++) {
            discreteDividendLength = boost::posix_time::minutes(discreteDividendsTimeUntil[i] * 360 * 24 * 60);
            QuantLib::Date dt(today.dateTime() + discreteDividendLength);
            discreteDividendDates.push_back(dt);
        }
    }
#else
    QuantLib::Date exDate = today + length;
    
    if(discreteDividends[0] != 0) {
        for (int i = 0; i < n; i++) {
            discreteDividendDates[i] = today + int(discreteDividendsTimeUntil[i] * 360 + 0.5); 
        }
    }
#endif    
    boost::shared_ptr<QuantLib::Exercise> exercise(new QuantLib::EuropeanExercise(exDate));

    boost::shared_ptr<QuantLib::StrikedTypePayoff> payoff(new QuantLib::PlainVanillaPayoff(optionType, strike));
    
    if(discreteDividends[0] != 0) { 
        boost::shared_ptr<QuantLib::BlackScholesMertonProcess> 
            stochProcess(new QuantLib::BlackScholesMertonProcess(QuantLib::Handle<QuantLib::Quote>(spot),
                                                                 QuantLib::Handle<QuantLib::YieldTermStructure>(qTS),
                                                                 QuantLib::Handle<QuantLib::YieldTermStructure>(rTS),
                                                                 QuantLib::Handle<QuantLib::BlackVolTermStructure>(volTS)));
        
        boost::shared_ptr<QuantLib::PricingEngine> engine(
                new QuantLib::AnalyticDividendEuropeanEngine(stochProcess));
        
        
        QuantLib::DividendVanillaOption option(payoff, exercise,
                                               discreteDividendDates, discreteDividends);
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
        boost::shared_ptr<QuantLib::VanillaOption> option = makeOption(payoff, exercise, spot, qTS, rTS, volTS);
        
        return Rcpp::List::create(Rcpp::Named("value") = option->NPV(),
                                  Rcpp::Named("delta") = option->delta(),
                                  Rcpp::Named("gamma") = option->gamma(),
                                  Rcpp::Named("vega") = option->vega(),
                                  Rcpp::Named("theta") = option->theta(),
                                  Rcpp::Named("rho") = option->rho(),
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
                                std::vector<double> discreteDividends,
                                std::vector<double> discreteDividendsTimeUntil) {
    
    

#ifdef QL_HIGH_RESOLUTION_DATE    
    // in minutes
    boost::posix_time::time_duration length = boost::posix_time::minutes(maturity * 360 * 24 * 60); 
#else
    int length = int(maturity * 360 + 0.5); // FIXME: this could be better
    
#endif
    QuantLib::Option::Type optionType = getOptionType(type);

    // new framework as per QuantLib 0.3.5, updated for 0.3.7
    // updated again for 0.9.0, see eg test-suite/americanoption.cpp
    QuantLib::Date today = QuantLib::Date::todaysDate();
    QuantLib::Settings::instance().evaluationDate() = today;
    QuantLib::DayCounter dc = QuantLib::Actual360();
    boost::shared_ptr<QuantLib::SimpleQuote> spot(new QuantLib::SimpleQuote(underlying));
    boost::shared_ptr<QuantLib::SimpleQuote> qRate(new QuantLib::SimpleQuote(dividendYield));
    boost::shared_ptr<QuantLib::YieldTermStructure> qTS = flatRate(today,qRate,dc);
    boost::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote(riskFreeRate));
    boost::shared_ptr<QuantLib::YieldTermStructure> rTS = flatRate(today,rRate,dc);
    boost::shared_ptr<QuantLib::SimpleQuote> vol(new QuantLib::SimpleQuote(volatility));
    boost::shared_ptr<QuantLib::BlackVolTermStructure> volTS = flatVol(today, vol, dc);
    
    boost::shared_ptr<QuantLib::StrikedTypePayoff> payoff(new QuantLib::PlainVanillaPayoff(optionType, strike));

    int n = discreteDividendsTimeUntil.size();
    std::vector<QuantLib::Date> discreteDividendDates;
    
#ifdef QL_HIGH_RESOLUTION_DATE
    QuantLib::Date exDate(today.dateTime() + length);

    if(discreteDividends[0] != 0) {
        boost::posix_time::time_duration discreteDividendLength;
        for (int i = 0; i < n; i++) {
            discreteDividendLength = boost::posix_time::minutes(discreteDividendsTimeUntil[i] * 360 * 24 * 60);
            QuantLib::Date dt(today.dateTime() + discreteDividendLength);
            discreteDividendDates.push_back(dt);
        }
    }
#else
    QuantLib::Date exDate = today + length;
    
    if(discreteDividends[0] != 0) {
        std::vector<int> discreteDividendLengths;
        for (int i = 0; i < n; i++) {
            discreteDividendLengths.push_back(int(discreteDividendsTimeUntil[i] * 360 + 0.5));
            discreteDividendDates.push_back(today + discreteDividendLengths[i]); 
        }
    }
#endif    
    boost::shared_ptr<QuantLib::Exercise> exercise(new QuantLib::AmericanExercise(today, exDate));
    

    boost::shared_ptr<QuantLib::BlackScholesMertonProcess> 
        stochProcess(new QuantLib::BlackScholesMertonProcess(QuantLib::Handle<QuantLib::Quote>(spot),
                                                             QuantLib::Handle<QuantLib::YieldTermStructure>(qTS),
                                                             QuantLib::Handle<QuantLib::YieldTermStructure>(rTS),
                                                             QuantLib::Handle<QuantLib::BlackVolTermStructure>(volTS)));

    if(discreteDividends[0] != 0) { 
        QuantLib::DividendVanillaOption option(payoff, exercise,
                                               discreteDividendDates, discreteDividends);
        if (engine=="BaroneAdesiWhaley") { 
            Rf_warning("Discrete dividends, engine switched to CrankNicolson");
            engine = "CrankNicolson";
        }
        
        if (engine=="CrankNicolson") { // FDDividendAmericanEngine only works with CrankNicolson
            // suggestion by Bryan Lewis: use CrankNicolson for greeks
            boost::shared_ptr<QuantLib::PricingEngine> 
            fdcnengine(new QuantLib::FDDividendAmericanEngine<QuantLib::CrankNicolson>(stochProcess, timeSteps, gridPoints));
            option.setPricingEngine(fdcnengine);
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
            
            boost::shared_ptr<QuantLib::PricingEngine> engine(new QuantLib::BaroneAdesiWhaleyApproximationEngine(stochProcess));
            option.setPricingEngine(engine);
            return Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                      Rcpp::Named("delta") = R_NaReal,
                                      Rcpp::Named("gamma") = R_NaReal,
                                      Rcpp::Named("vega") = R_NaReal,
                                      Rcpp::Named("theta") = R_NaReal,
                                      Rcpp::Named("rho") = R_NaReal,
                                      Rcpp::Named("divRho") = R_NaReal);
        } else if (engine=="CrankNicolson") {
            // suggestion by Bryan Lewis: use CrankNicolson for greeks
            boost::shared_ptr<QuantLib::PricingEngine> 
            fdcnengine(new QuantLib::FDAmericanEngine<QuantLib::CrankNicolson>(stochProcess, timeSteps, gridPoints));
            option.setPricingEngine(fdcnengine);
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
#ifdef QL_HIGH_RESOLUTION_DATE    
        // in minutes
        boost::posix_time::time_duration length = boost::posix_time::minutes(maturity * 360 * 24 * 60); 
#else
        int length           = int(maturity*360 + 0.5); // FIXME: this could be better
#endif
        double volatility    = par(i, 5);    // sixth column
    
        boost::shared_ptr<QuantLib::SimpleQuote> spot(new QuantLib::SimpleQuote( underlying ));
        boost::shared_ptr<QuantLib::SimpleQuote> vol(new QuantLib::SimpleQuote( volatility ));
        boost::shared_ptr<QuantLib::BlackVolTermStructure> volTS = flatVol(today, vol, dc);
        boost::shared_ptr<QuantLib::SimpleQuote> qRate(new QuantLib::SimpleQuote( dividendYield ));
        boost::shared_ptr<QuantLib::YieldTermStructure> qTS = flatRate(today, qRate, dc);
        boost::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote( riskFreeRate ));
        boost::shared_ptr<QuantLib::YieldTermStructure> rTS = flatRate(today, rRate, dc);
        
#ifdef QL_HIGH_RESOLUTION_DATE
    QuantLib::Date exDate(today.dateTime() + length);
#else
    QuantLib::Date exDate = today + length;
#endif    
        boost::shared_ptr<QuantLib::Exercise> exercise(new QuantLib::EuropeanExercise(exDate));
        
        boost::shared_ptr<QuantLib::StrikedTypePayoff> payoff(new QuantLib::PlainVanillaPayoff(optionType, strike));
        boost::shared_ptr<QuantLib::VanillaOption> option = makeOption(payoff, exercise, spot, qTS, rTS, volTS);
        
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
