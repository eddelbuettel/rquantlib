// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib -- R interface to the QuantLib libraries
//
// Copyright (C) 2002 - 2009  Dirk Eddelbuettel 
// Copyright (C) 2009 - 2010  Dirk Eddelbuettel and Khanh Nguyen
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

#include <rquantlib.hpp>

RcppExport SEXP QL_AsianOption(SEXP optionParameters){

    try{
        Rcpp::List rparam(optionParameters);

        std::string avgType = Rcpp::as<std::string>(rparam["averageType"]);
        std::string type = Rcpp::as<std::string>(rparam["type"]);
        double underlying = Rcpp::as<double>(rparam["underlying"]);
        double strike = Rcpp::as<double>(rparam["strike"]);
        Spread dividendYield = Rcpp::as<double>(rparam["dividendYield"]);
        Rate riskFreeRate = Rcpp::as<double>(rparam["riskFreeRate"]);
        Time maturity = Rcpp::as<double>(rparam["maturity"]);
        //        int length = int(maturity*360 + 0.5); // FIXME: this could be better
        double volatility = Rcpp::as<double>(rparam["volatility"]);

        Option::Type optionType = getOptionType(type);

    
        

        //from test-suite/asionoptions.cpp
        DayCounter dc = Actual360();
        Date today = Date::todaysDate();
        Settings::instance().evaluationDate() = today;

        boost::shared_ptr<SimpleQuote> spot(new SimpleQuote(underlying));
        boost::shared_ptr<SimpleQuote> qRate(new SimpleQuote(dividendYield));
        boost::shared_ptr<YieldTermStructure> qTS = flatRate(today, qRate, dc);
        boost::shared_ptr<SimpleQuote> rRate(new SimpleQuote(riskFreeRate));
        boost::shared_ptr<YieldTermStructure> rTS = flatRate(today, rRate, dc);
        boost::shared_ptr<SimpleQuote> vol(new SimpleQuote(volatility));
        boost::shared_ptr<BlackVolTermStructure> volTS = flatVol(today, vol, dc);
        
        boost::shared_ptr<BlackScholesMertonProcess>
            stochProcess(new
                         BlackScholesMertonProcess(Handle<Quote>(spot),
                                                   Handle<YieldTermStructure>(qTS),
                                                   Handle<YieldTermStructure>(rTS),
                                                   Handle<BlackVolTermStructure>(volTS)));

        boost::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(optionType,strike));

        Date exDate = today + int(maturity * 360 + 0.5);
        boost::shared_ptr<Exercise> exercise(new EuropeanExercise(exDate));
        

        Average::Type averageType = Average::Geometric;
        Rcpp::List rl = NULL;
   
        if (avgType=="geometric"){
            averageType = Average::Geometric;
            boost::shared_ptr<PricingEngine> 
                engine(new
                       AnalyticContinuousGeometricAveragePriceAsianEngine(stochProcess));        
            ContinuousAveragingAsianOption option(averageType, payoff, exercise);
            option.setPricingEngine(engine);
            
            rl = Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                    Rcpp::Named("delta") = option.delta(),
                                    Rcpp::Named("gamma") = option.gamma(),
                                    Rcpp::Named("vega") = option.vega(),
                                    Rcpp::Named("theta") = option.theta(),
                                    Rcpp::Named("rho") = option.rho(),
                                    Rcpp::Named("divRho") = option.dividendRho(),
                                    Rcpp::Named("parameters") = optionParameters);
            
        } else if (avgType=="arithmetic"){
            averageType = Average::Arithmetic;
            boost::shared_ptr<PricingEngine> engine =
                MakeMCDiscreteArithmeticASEngine<LowDiscrepancy>(stochProcess)
                .withSeed(3456789)
                .withSamples(1023);
            
            Size fixings = Rcpp::as<double>(rparam["fixings"]);
            Time length = Rcpp::as<double>(rparam["length"]);
            Time first = Rcpp::as<double>(rparam["first"]);
            Time dt = length / (fixings - 1);

            std::vector<Time> timeIncrements(fixings);
            std::vector<Date> fixingDates(fixings);
            timeIncrements[0] = first;
            fixingDates[0] = today + Integer(timeIncrements[0] * 360 + 0.5);
            for (Size i=1; i<fixings; i++) {
                timeIncrements[i] = i*dt + first;
                fixingDates[i] = today + Integer(timeIncrements[i]*360+0.5);
            }
            Real runningSum = 0.0;
            Size pastFixing = 0;

            DiscreteAveragingAsianOption option(Average::Arithmetic, 
                                                runningSum,
                                                pastFixing, 
                                                fixingDates,
                                                payoff, exercise);
            option.setPricingEngine(engine);
            rl = Rcpp::List::create(Rcpp::Named("value") = option.NPV(),
                                    Rcpp::Named("delta") = 0,
                                    Rcpp::Named("gamma") = 0,
                                    Rcpp::Named("vega") = 0,
                                    Rcpp::Named("theta") = 0 ,
                                    Rcpp::Named("rho") = 0,
                                    Rcpp::Named("divRho") = 0,
                                    Rcpp::Named("parameters") = optionParameters);
        } else {
            throw std::range_error("Unknown average type " + type);
        }      
    
        return rl;

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;
}

