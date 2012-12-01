// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2002 - 2010 Dirk Eddelbuettel <edd@debian.org>
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

#include <rquantlib.h>

RcppExport SEXP EuropeanOption(SEXP optionParameters) {

    try {

        Rcpp::List rparam(optionParameters);    	// Parameters via R list object

        std::string type     = Rcpp::as<std::string>(rparam["type"]);
        double underlying    = Rcpp::as<double>(rparam["underlying"]);
        double strike        = Rcpp::as<double>(rparam["strike"]);
        QuantLib::Spread dividendYield = Rcpp::as<double>(rparam["dividendYield"]);
        QuantLib::Rate riskFreeRate    = Rcpp::as<double>(rparam["riskFreeRate"]);
        QuantLib::Time maturity        = Rcpp::as<double>(rparam["maturity"]);
        int length           = int(maturity*360 + 0.5); // FIXME: this could be better
        double volatility    = Rcpp::as<double>(rparam["volatility"]);
    
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

        QuantLib::Date exDate = today + length;
        boost::shared_ptr<QuantLib::Exercise> exercise(new QuantLib::EuropeanExercise(exDate));
	
        boost::shared_ptr<QuantLib::StrikedTypePayoff> payoff(new QuantLib::PlainVanillaPayoff(optionType, strike));
        boost::shared_ptr<QuantLib::VanillaOption> option = makeOption(payoff, exercise, spot, qTS, rTS, volTS);

        Rcpp::List rl = Rcpp::List::create(Rcpp::Named("value") = option->NPV(),
                                           Rcpp::Named("delta") = option->delta(),
                                           Rcpp::Named("gamma") = option->gamma(),
                                           Rcpp::Named("vega") = option->vega(),
                                           Rcpp::Named("theta") = option->theta(),
                                           Rcpp::Named("rho") = option->rho(),
                                           Rcpp::Named("divRho") = option->dividendRho(),
                                           Rcpp::Named("parameters") = optionParameters);
        return rl;

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP AmericanOption(SEXP optionParameters) {

    try {
        Rcpp::List rparam(optionParameters);

        std::string type = Rcpp::as<std::string>(rparam["type"]);
        double underlying = Rcpp::as<double>(rparam["underlying"]);
        double strike = Rcpp::as<double>(rparam["strike"]);
        QuantLib::Spread dividendYield = Rcpp::as<double>(rparam["dividendYield"]);
        QuantLib::Rate riskFreeRate = Rcpp::as<double>(rparam["riskFreeRate"]);
        QuantLib::Time maturity = Rcpp::as<double>(rparam["maturity"]);
        int length = int(maturity*360 + 0.5); // FIXME: this could be better
        double volatility = Rcpp::as<double>(rparam["volatility"]);
        int timeSteps = Rcpp::as<int>(rparam["timeSteps"]);
        int gridPoints = Rcpp::as<int>(rparam["gridPoints"]);
        std::string engine = Rcpp::as<std::string>(rparam["engine"]);

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

        QuantLib::Date exDate = today + length;
        boost::shared_ptr<QuantLib::Exercise> exercise(new QuantLib::AmericanExercise(today, exDate));

        boost::shared_ptr<QuantLib::BlackScholesMertonProcess> 
            stochProcess(new QuantLib::BlackScholesMertonProcess(QuantLib::Handle<QuantLib::Quote>(spot),
                                                                 QuantLib::Handle<QuantLib::YieldTermStructure>(qTS),
                                                                 QuantLib::Handle<QuantLib::YieldTermStructure>(rTS),
                                                                 QuantLib::Handle<QuantLib::BlackVolTermStructure>(volTS)));

        QuantLib::VanillaOption option(payoff, exercise);
        if (engine=="BaroneAdesiWhaley") {
            // new from 0.3.7 BaroneAdesiWhaley
            boost::shared_ptr<QuantLib::PricingEngine> engine(new QuantLib::BaroneAdesiWhaleyApproximationEngine(stochProcess));
            option.setPricingEngine(engine);
            Rcpp::List rl = Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                               Rcpp::Named("delta") = R_NaReal,
                                               Rcpp::Named("gamma") = R_NaReal,
                                               Rcpp::Named("vega") = R_NaReal,
                                               Rcpp::Named("theta") = R_NaReal,
                                               Rcpp::Named("rho") = R_NaReal,
                                               Rcpp::Named("divRho") = R_NaReal,
                                               Rcpp::Named("parameters") = optionParameters);
            return rl;
        } else if (engine=="CrankNicolson") {
            // suggestion by Bryan Lewis: use CrankNicolson for greeks
            boost::shared_ptr<QuantLib::PricingEngine> 
                fdcnengine(new QuantLib::FDAmericanEngine<QuantLib::CrankNicolson>(stochProcess, timeSteps, gridPoints));
            option.setPricingEngine(fdcnengine);
            Rcpp::List rl = Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                               Rcpp::Named("delta") = option.delta(),
                                               Rcpp::Named("gamma") = option.gamma(),
                                               Rcpp::Named("vega") = R_NaReal,
                                               Rcpp::Named("theta") = R_NaReal,
                                               Rcpp::Named("rho") = R_NaReal,
                                               Rcpp::Named("divRho") = R_NaReal,
                                               Rcpp::Named("parameters") = optionParameters);
            return rl;
        } else {
            throw std::range_error("Unknown engine " + engine);
        }
    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

RcppExport SEXP EuropeanOptionArrays(SEXP typesexp, SEXP parsexp) {

    try {
        QuantLib::Option::Type optionType = getOptionType( Rcpp::as<std::string>(typesexp) );
        Rcpp::NumericMatrix par(parsexp); // matrix of parameters as per expand.grid() in R
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
            int length           = int(maturity*360 + 0.5); // FIXME: this could be better
            double volatility    = par(i, 5);    // sixth column
    
            boost::shared_ptr<QuantLib::SimpleQuote> spot(new QuantLib::SimpleQuote( underlying ));
            boost::shared_ptr<QuantLib::SimpleQuote> vol(new QuantLib::SimpleQuote( volatility ));
            boost::shared_ptr<QuantLib::BlackVolTermStructure> volTS = flatVol(today, vol, dc);
            boost::shared_ptr<QuantLib::SimpleQuote> qRate(new QuantLib::SimpleQuote( dividendYield ));
            boost::shared_ptr<QuantLib::YieldTermStructure> qTS = flatRate(today, qRate, dc);
            boost::shared_ptr<QuantLib::SimpleQuote> rRate(new QuantLib::SimpleQuote( riskFreeRate ));
            boost::shared_ptr<QuantLib::YieldTermStructure> rTS = flatRate(today, rRate, dc);

            QuantLib::Date exDate = today + length;
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

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}
