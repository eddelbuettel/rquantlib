/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2012 Peter Caspers
 
 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/
 
 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.
 
 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
 */
#include "rquantlib.h"

#include "markovfunctional.hpp"
#include "utilities.hpp"
#include <ql/processes/mfstateprocess.hpp>
#include <ql/models/shortrate/onefactormodels/markovfunctional.hpp>
#include <ql/pricingengines/swaption/gaussian1dswaptionengine.hpp>
#include <ql/pricingengines/capfloor/gaussian1dcapfloorengine.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <ql/termstructures/yield/piecewiseyieldcurve.hpp>
#include <ql/termstructures/volatility/swaption/swaptionconstantvol.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/termstructures/volatility/swaption/swaptionvolmatrix.hpp>
#include <ql/termstructures/volatility/swaption/swaptionvolcube1.hpp>
#include <ql/termstructures/volatility/swaption/swaptionvolcube2.hpp>
#include <ql/termstructures/volatility/capfloor/capfloortermvolsurface.hpp>
#include <ql/termstructures/volatility/optionlet/optionletstripper1.hpp>
#include <ql/termstructures/volatility/optionlet/strippedoptionletadapter.hpp>
#include <ql/termstructures/volatility/interpolatedsmilesection.hpp>
#include <ql/termstructures/volatility/kahalesmilesection.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/indexes/swap/euriborswap.hpp>
#include <ql/indexes/ibor/euribor.hpp>
#include <ql/termstructures/yield/ratehelpers.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/thirty360.hpp>
#include <ql/time/daycounters/actualactual.hpp>
#include <ql/instruments/makeswaption.hpp>
#include <ql/instruments/makevanillaswap.hpp>
#include <ql/instruments/makecapfloor.hpp>
#include <ql/cashflows/cashflowvectors.hpp>
#include <ql/pricingengines/swaption/blackswaptionengine.hpp>
#include <ql/pricingengines/capfloor/blackcapfloorengine.hpp>
#include <ql/models/shortrate/calibrationhelpers/swaptionhelper.hpp>
#include <ql/models/shortrate/calibrationhelpers/caphelper.hpp>

using namespace QuantLib;
//using namespace boost::unit_test_framework;

using std::fabs;


namespace {

Handle<SwaptionVolatilityStructure> swptnVolCube(                    Rcpp::NumericVector &expiries,
                                                                        Rcpp::NumericVector &tenors,
                                                                        Rcpp::NumericMatrix &atmVols,
                                                                        Rcpp::NumericVector &strikes,
                                                                        Rcpp::NumericMatrix &smirkVols, 
                                                                        QuantLib::Handle<QuantLib::YieldTermStructure>  &yldCrv
) {
  
  std::vector<Period> optionTenors;
  std::vector<Period> swapTenors;
  
  
  QuantLib::Size numExp = expiries.size();
  for (Size i = 0; i <numExp ; i++){
    if(expiries[i]<1.0){
      int n = static_cast<int>(12*expiries[i]);

      optionTenors.push_back(Months*n);
      
    } else {
      int n = static_cast<int>(expiries[i]);

      optionTenors.push_back(Years*n);
    }
  }
  
  QuantLib::Size numTenor = tenors.size();
  for (Size i = 0; i < numTenor; i++){
    if(tenors[i]<1.0){
      int n = static_cast<int>(12*tenors[i]);
      swapTenors.push_back(Months*n);
      
    } else {
      int n = static_cast<int>(tenors[i]);
      swapTenors.push_back(Years*n);
    }
  }
  
  
  
  std::vector<std::vector<Handle<Quote> > > qSwAtm;
  
  for (int i = 0; i < numExp; i++) {
    std::vector<Handle<Quote> > qSwAtmTmp;
    for (int j = 0; j < numTenor; j++) {
      double qt=atmVols(i,j);
      qSwAtmTmp.push_back(Handle<Quote>(boost::shared_ptr<Quote>(
          new SimpleQuote(qt))));
    }
    qSwAtm.push_back(qSwAtmTmp);
  }

  Handle<SwaptionVolatilityStructure> swaptionVolAtm(
      boost::shared_ptr<SwaptionVolatilityStructure>(
        new SwaptionVolatilityMatrix(TARGET(), ModifiedFollowing,
                                     optionTenors, swapTenors, qSwAtm,
                                     Actual365Fixed())));

  std::vector<Period> optionTenorsSmile;
  std::vector<Period> swapTenorsSmile;
  std::vector<Real> strikeSpreads;
  
  for (Size i = 0; i < numExp; i++){
    if(expiries[i]<1.0){
      int n = static_cast<int>(12*expiries[i]);
    optionTenorsSmile.push_back(n*Months);
    } else {
      int n = static_cast<int>(expiries[i]);
      optionTenorsSmile.push_back(n*Years);
    }
  }
  for (Size i = 0; i < numTenor; i++){
    if(tenors[i]<1.0){
      int n = static_cast<int>(12*tenors[i]);
      swapTenorsSmile.push_back(Months*n);
      
    } else {
      int n = static_cast<int>(tenors[i]);
      swapTenorsSmile.push_back(Years*n);
    }

  }
  QuantLib::Size numStrike = strikes.size();
  for (Size i = 0; i < numStrike; i++)
    strikeSpreads.push_back(strikes[i]);
  
  std::vector<std::vector<Handle<Quote> > > qSwSmile;
  
  for (int i = 0; i < numExp*numTenor; i++) {
    std::vector<Handle<Quote> > qSwSmileTmp;
    for (int j = 0; j < numStrike; j++) {
      double qt=smirkVols(i,j);
      qSwSmileTmp.push_back(Handle<Quote>(boost::shared_ptr<Quote>(
      new SimpleQuote(qt))));
    }
    qSwSmile.push_back(qSwSmileTmp);
  }
  
  double qSwSmileh1[] = {
    0.01, 0.2, 0.8, -0.2
  };
  
  std::vector<bool> parameterFixed;
  parameterFixed.push_back(false);
  parameterFixed.push_back(false); // beta could be fixed
  parameterFixed.push_back(false);
  parameterFixed.push_back(false);

  std::vector<std::vector<Handle<Quote> > > parameterGuess;
  for (int i = 0; i < numExp*numTenor; i++) {
    std::vector<Handle<Quote> > parameterGuessTmp;
    for (int j = 0; j < 4; j++) {
      parameterGuessTmp.push_back(
        Handle<Quote>(boost::shared_ptr<Quote>(
            new SimpleQuote(qSwSmileh1[j]))));
    }
    parameterGuess.push_back(parameterGuessTmp);
  }

  boost::shared_ptr<EndCriteria> ec(
      new EndCriteria(5000000, 250, 1E-6, 1E-6, 1E-6));
  
  boost::shared_ptr<SwapIndex> swapIndex(new EuriborSwapIsdaFixA(
      30 * Years, Handle<YieldTermStructure>(yldCrv)));
  boost::shared_ptr<SwapIndex> shortSwapIndex(new EuriborSwapIsdaFixA(
      1 * Years,
      Handle<YieldTermStructure>(yldCrv))); // We assume that we have 6m
  // vols (which we actually
  // don't have for 1y
  // underlying, but this is
  // just a test...)

  
  // return Handle<SwaptionVolatilityStructure>(new
  // SwaptionVolCube2(swaptionVolAtm,optionTenorsSmile,swapTenorsSmile,strikeSpreads,qSwSmile,swapIndex,shortSwapIndex,false));
  // // bilinear interpolation gives nasty digitals
  Handle<SwaptionVolatilityStructure> res(
      boost::shared_ptr<SwaptionVolatilityStructure>(new SwaptionVolCube1(
          swaptionVolAtm, optionTenorsSmile, swapTenorsSmile,
          strikeSpreads, qSwSmile, swapIndex, shortSwapIndex, true,
          parameterGuess, parameterFixed, true, ec,
          .050))); // put a big error tolerance here ... we just want a
  // smooth cube for testing

  res->enableExtrapolation();

  return res;
}
  

 }  /// closes name space (i think)


//[[Rcpp::export]]
Rcpp::List sabrengine(Rcpp::List rparam,
                              Rcpp::List legparams,
                              std::vector<QuantLib::Date> dateVec, 
                              std::vector<double> zeroVec,
                              Rcpp::NumericVector swaptionMat,
                              Rcpp::NumericVector swapLengths,
                              Rcpp::NumericMatrix atmVols,
                              Rcpp::NumericVector strikes,
                              Rcpp::NumericMatrix smirkVols){
  
  QuantLib::Date todaysDate(Rcpp::as<QuantLib::Date>(rparam["tradeDate"])); 
  QuantLib::Date settlementDate(Rcpp::as<QuantLib::Date>(rparam["settleDate"])); 
  QuantLib::Date startDate(Rcpp::as<QuantLib::Date>(rparam["startDate"])); 
  QuantLib::Date maturity(Rcpp::as<QuantLib::Date>(rparam["maturity"])); 
  bool payfix = Rcpp::as<bool>(rparam["payFixed"]);
  bool european = Rcpp::as<bool>(rparam["european"]);
  double strike = Rcpp::as<double>(rparam["strike"]);
  
  
  const Real tol1 = 0.0001; // 1bp tolerance for model engine call put premia
  // vs. black premia
  // note that we use the real market conventions here (i.e. 2 fixing days),
  // different from the calibration approach where 0 fixing days must be used.
  // therefore higher errors compared to the calibration results are expected.
  
  // BOOST_TEST_MESSAGE("Testing Markov functional vanilla engines...");
  
  Date savedEvalDate = Settings::instance().evaluationDate();
  Settings::instance().evaluationDate() = todaysDate;

  QuantLib::Handle<QuantLib::YieldTermStructure> yldCrv(rebuildCurveFromZeroRates(dateVec, zeroVec));

  Handle<SwaptionVolatilityStructure> volCube = swptnVolCube(swaptionMat,swapLengths,atmVols,
                                                                 strikes,smirkVols,yldCrv);
  
  boost::shared_ptr<IborIndex> iborIndex1(new Euribor(6 * Months, yldCrv));
  
  // boost::shared_ptr<MarkovFunctional> mf1(new MarkovFunctional(
  //     flatYts_, 0.01, volStepDates, vols, flatSwaptionVts_,
  //     expiriesCalBasket1(), tenorsCalBasket1(), swapIndexBase,
  //     MarkovFunctional::ModelSettings()
  //                                           .withYGridPoints(64)
  //                                           .withYStdDevs(7.0)
  //                                           .withGaussHermitePoints(32)
  //                                           .withDigitalGap(1e-5)
  //                                           .withMarketRateAccuracy(1e-7)
  //                                           .withLowerRateBound(0.0)
  //                                           .withUpperRateBound(2.0)
  //                                           .withSmileMoneynessCheckpoints(money)));
  //     
  //     MarkovFunctional::ModelOutputs outputs1 = mf1->modelOutputs();
  //     // BOOST_TEST_MESSAGE(outputs1);
  //     
  //     boost::shared_ptr<Gaussian1dSwaptionEngine> mfSwaptionEngine1(
  //         new Gaussian1dSwaptionEngine(mf1, 64, 7.0));
      boost::shared_ptr<BlackSwaptionEngine> blackSwaptionEngine1(
                       new BlackSwaptionEngine(yldCrv, volCube));
          
      Real vol, blackPriceCall,rate;

          boost::shared_ptr<VanillaSwap> underlyingCall =
            MakeVanillaSwap(Years*(daysBetween(startDate,maturity)/365.0), iborIndex1,
                            strike)
                                    .withEffectiveDate(
                            startDate)
                                    .receiveFixed(false);
                            boost::shared_ptr<VanillaSwap> underlyingPut =
                            MakeVanillaSwap(Years*(daysBetween(startDate,maturity)/365.0), iborIndex1,
                                            strike)
                                                    .withEffectiveDate(
                                            startDate)
                                                    .receiveFixed(true);

                                            boost::shared_ptr<Exercise> exercise(
                                                new EuropeanExercise(startDate));

                                       //     Rprintf("%d %d %d\n",outputs1.expiries_[i].dayOfMonth(),outputs1.expiries_[i].month(),outputs1.expiries_[i].year());
                                            vol=volCube->volatility(
                                              Years*(daysBetween(settlementDate,startDate)/365.0), Years*(daysBetween(startDate,maturity)/365.0),
                                              strike);
                                                boost::shared_ptr<PricingEngine> swapEngine(new DiscountingSwapEngine(yldCrv));
                                                
                                                underlyingCall->setPricingEngine(swapEngine);
                                                rate=underlyingCall->fairRate();
                                            Swaption swaptionC(underlyingCall, exercise);
                                            Swaption swaptionP(underlyingPut, exercise);
                                            swaptionC.setPricingEngine(blackSwaptionEngine1);
                                            swaptionP.setPricingEngine(blackSwaptionEngine1);
                                            blackPriceCall = swaptionC.NPV();
                                            Real blackPricePut = swaptionP.NPV();

      return Rcpp::List::create(Rcpp::Named("call") = blackPriceCall,Rcpp::Named("put") = blackPricePut,
                                Rcpp::Named("sigma") = vol,Rcpp::Named("atmRate") = rate);
}