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

#include <rquantlib.h>

RcppExport SEXP AsianOption(SEXP optionParameters){

    try{
        Rcpp::List rparam(optionParameters);

        std::string avgType = Rcpp::as<std::string>(rparam["averageType"]);
        std::string type = Rcpp::as<std::string>(rparam["type"]);
        double underlying = Rcpp::as<double>(rparam["underlying"]);
        double strike = Rcpp::as<double>(rparam["strike"]);
        QuantLib::Spread dividendYield = Rcpp::as<double>(rparam["dividendYield"]);
        QuantLib::Rate riskFreeRate = Rcpp::as<double>(rparam["riskFreeRate"]);
        QuantLib::Time maturity = Rcpp::as<double>(rparam["maturity"]);
        //        int length = int(maturity*360 + 0.5); // FIXME: this could be better
        double volatility = Rcpp::as<double>(rparam["volatility"]);

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

        boost::shared_ptr<QuantLib::StrikedTypePayoff> payoff(new QuantLib::PlainVanillaPayoff(optionType,strike));

      

        QuantLib::Average::Type averageType = QuantLib::Average::Geometric;
        Rcpp::List rl = R_NilValue;
   
        if (avgType=="geometric"){
            averageType = QuantLib::Average::Geometric;
            boost::shared_ptr<QuantLib::PricingEngine> 
                engine(new
                       QuantLib::AnalyticContinuousGeometricAveragePriceAsianEngine(stochProcess));
            
            QuantLib::Date exDate = today + int(maturity * 360 + 0.5);
            boost::shared_ptr<QuantLib::Exercise> exercise(new QuantLib::EuropeanExercise(exDate));
            QuantLib::ContinuousAveragingAsianOption option(averageType, payoff, exercise);
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
            averageType = QuantLib::Average::Arithmetic;

            boost::shared_ptr<QuantLib::PricingEngine> engine =
                QuantLib::MakeMCDiscreteArithmeticAPEngine<QuantLib::LowDiscrepancy>(stochProcess)
                .withSamples(2047)
                .withControlVariate();
            
            //boost::shared_ptr<PricingEngine> engine =
            //    MakeMCDiscreteArithmeticASEngine<LowDiscrepancy>(stochProcess)
            //    .withSeed(3456789)
            //    .withSamples(1023);
            
            QuantLib::Size fixings = Rcpp::as<double>(rparam["fixings"]);
            QuantLib::Time length = Rcpp::as<double>(rparam["length"]);
            QuantLib::Time first = Rcpp::as<double>(rparam["first"]);
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
                                    Rcpp::Named("divRho") = R_NaN,
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

