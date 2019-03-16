
/*
 Copyright (C) 2012 Peter Caspers
 Copyright (C) 2016 Terry Leitch

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

#include <rquantlib_internal.h>

using namespace QuantLib;
//using namespace boost::unit_test_framework;

using std::fabs;

namespace {

    typedef SwaptionVolatilityStructure SwpVolStr; // shorthand used below
    typedef SwaptionVolatilityMatrix SwpVolMat;    // shorthand used below

    Handle<SwpVolStr> swptnVolCube(Rcpp::NumericVector &expiries,
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

        for (unsigned int i = 0; i < numExp; i++) {
            std::vector<Handle<Quote> > qSwAtmTmp;
            for (unsigned int j = 0; j < numTenor; j++) {
                double qt=atmVols(i,j);
                qSwAtmTmp.push_back(Handle<Quote>(QuantLib::ext::shared_ptr<Quote>(new SimpleQuote(qt))));
            }
            qSwAtm.push_back(qSwAtmTmp);
        }

        Handle<SwpVolStr>
        swaptionVolAtm(QuantLib::ext::shared_ptr<SwpVolStr>(new SwpVolMat(TARGET(),
                                                                  ModifiedFollowing,
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

        for (unsigned int i = 0; i < numExp*numTenor; i++) {
            std::vector<Handle<Quote> > qSwSmileTmp;
            for (unsigned int j = 0; j < numStrike; j++) {
                double qt=smirkVols(i,j);
                qSwSmileTmp.push_back(Handle<Quote>(QuantLib::ext::shared_ptr<Quote>(new SimpleQuote(qt))));
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
        for (unsigned int i = 0; i < numExp*numTenor; i++) {
            std::vector<Handle<Quote> > parameterGuessTmp;
            for (unsigned int j = 0; j < 4; j++) {
                parameterGuessTmp.push_back(Handle<Quote>(QuantLib::ext::shared_ptr<Quote>(new SimpleQuote(qSwSmileh1[j]))));
            }
            parameterGuess.push_back(parameterGuessTmp);
        }

        QuantLib::ext::shared_ptr<EndCriteria> ec(new EndCriteria(5000000, 250, 1E-6, 1E-6, 1E-6));

        QuantLib::ext::shared_ptr<SwapIndex> swapIndex(new EuriborSwapIsdaFixA(30 * Years, Handle<YieldTermStructure>(yldCrv)));
        QuantLib::ext::shared_ptr<SwapIndex> shortSwapIndex(new EuriborSwapIsdaFixA(1 * Years, Handle<YieldTermStructure>(yldCrv)));


        Handle<SwpVolStr>
            res(QuantLib::ext::shared_ptr<SwpVolStr>(new SwaptionVolCube1(swaptionVolAtm, optionTenorsSmile,
                                                                  swapTenorsSmile, strikeSpreads, qSwSmile,
                                                                  swapIndex, shortSwapIndex, true,
                                                                  parameterGuess, parameterFixed, true, ec,
                                                                  .050)));
        // put a big error tolerance here ... we just want a
        // smooth cube for testing

        res->enableExtrapolation();

        return res;
    }

}  /// closes name space


// [[Rcpp::export]]
Rcpp::List sabrengine(Rcpp::List rparam,
                      Rcpp::List legParams,
                      std::vector<QuantLib::Date> dateVec,
                      std::vector<double> zeroVec,
                      Rcpp::NumericVector swaptionMat,
                      Rcpp::NumericVector swapLengths,
                      Rcpp::NumericMatrix atmVols,
                      Rcpp::NumericVector strikes,
                      Rcpp::NumericMatrix smirkVols){

    QuantLib::Date tradeDate(Rcpp::as<QuantLib::Date>(rparam["tradeDate"]));
    QuantLib::Date settlementDate(Rcpp::as<QuantLib::Date>(rparam["settleDate"]));
    QuantLib::Date startDate(Rcpp::as<QuantLib::Date>(rparam["startDate"]));
    QuantLib::Date expiryDate(Rcpp::as<QuantLib::Date>(rparam["expiryDate"]));
    QuantLib::Date maturity(Rcpp::as<QuantLib::Date>(rparam["maturity"]));
    bool european = Rcpp::as<bool>(rparam["european"]);
    double strike = Rcpp::as<double>(rparam["strike"]);
    //double fixDayCount = Rcpp::as<double>(legParams["dayCounter"]); // currently not used, fix in future
    double fixFreq   = Rcpp::as<double>(legParams["fixFreq"]) ;
    int floatFreq = Rcpp::as<int>(legParams["floatFreq"]);

    // BOOST_TEST_MESSAGE("Testing Markov functional vanilla engines...");

    Settings::instance().evaluationDate() = tradeDate;

    QuantLib::Handle<QuantLib::YieldTermStructure> yldCrv(rebuildCurveFromZeroRates(dateVec, zeroVec));

    Handle<SwaptionVolatilityStructure> volCube = swptnVolCube(swaptionMat,swapLengths,atmVols,
                                                               strikes,smirkVols,yldCrv);
    QuantLib::ext::shared_ptr<IborIndex> iborIndex1(new Euribor(floatFreq * Months, yldCrv));
    QuantLib::ext::shared_ptr<SwapIndex> swapIndexBase (new EuriborSwapIsdaFixA(fixFreq * Years));

    // create swaps for european swaption here to get atm fwd rate, these are ignored for bermudan  //
    QuantLib::ext::shared_ptr<VanillaSwap> underlyingCall =
        MakeVanillaSwap(Years*(((maturity-expiryDate)/365.0)), iborIndex1, strike)
        .withEffectiveDate(expiryDate)
        .receiveFixed(false);

    QuantLib::ext::shared_ptr<VanillaSwap> underlyingPut =
        MakeVanillaSwap(Years*((maturity-expiryDate)/365.0), iborIndex1, strike)
        .withEffectiveDate(expiryDate)
        .receiveFixed(true);
    QuantLib::ext::shared_ptr<PricingEngine> swapEngine(new DiscountingSwapEngine(yldCrv));

    underlyingCall->setPricingEngine(swapEngine);
    underlyingPut->setPricingEngine(swapEngine);

    Real vol,pricePay,priceRcv,rate;
    vol=volCube->volatility(Years*((expiryDate-settlementDate)/365.0), Years*((maturity-expiryDate)/365.0), strike);
    rate=underlyingCall->fairRate();
    // calculate if bermudan here //
    if(!european)  {
        QuantLib::ext::shared_ptr<VanillaSwap> underlyingCall2 =
            MakeVanillaSwap(Years*(((maturity-startDate)/365.0)), iborIndex1, strike)
            .withEffectiveDate(startDate)
            .receiveFixed(false);

        QuantLib::ext::shared_ptr<VanillaSwap> underlyingPut2 =
            MakeVanillaSwap(Years*((maturity-startDate)/365.0), iborIndex1, strike)
            .withEffectiveDate(startDate)
            .receiveFixed(true);
        QuantLib::ext::shared_ptr<PricingEngine> swapEngine(new DiscountingSwapEngine(yldCrv));

        underlyingCall2->setPricingEngine(swapEngine);
        underlyingPut2->setPricingEngine(swapEngine);
        std::vector<Date> volStepDates;
        std::vector<Real> vols;
        vols.push_back(1.0);
        std::vector<QuantLib::Date> exerciseDates;
        std::vector<QuantLib::Period> underlyingTenors;
        const std::vector<QuantLib::ext::shared_ptr<QuantLib::CashFlow> >& leg = underlyingCall2->fixedLeg();
        for (unsigned int i=0; i<leg.size(); i++) {
            QuantLib::ext::shared_ptr<QuantLib::Coupon> coupon = QuantLib::ext::dynamic_pointer_cast<QuantLib::Coupon>(leg[i]);
            if(coupon->accrualStartDate() <= expiryDate && coupon->accrualStartDate() >= startDate) {
                exerciseDates.push_back(coupon->accrualStartDate());
                underlyingTenors.push_back(Years*((maturity-(coupon->accrualStartDate()))/365.0));
            }
        }

        //   create expiry backet, get maturity
        QuantLib::ext::shared_ptr<MarkovFunctional>
            mf1(new MarkovFunctional(yldCrv, 0.01, volStepDates, vols,volCube,
                                     exerciseDates, underlyingTenors,
                                     swapIndexBase, MarkovFunctional::ModelSettings()
                                     .withYGridPoints(32)
                                     .withYStdDevs(7.0)
                                     .withGaussHermitePoints(16)
                                     .withMarketRateAccuracy(1e-7)
                                     .withDigitalGap(1e-5)
                                     .withLowerRateBound(0.0)
                                     .withUpperRateBound(2.0)));
        QuantLib::ext::shared_ptr<PricingEngine> mfSwaptionEngine1(new Gaussian1dSwaptionEngine(mf1, 64, 7.0));
        QuantLib::ext::shared_ptr<Exercise> bermudanExercise(new BermudanExercise(exerciseDates));

        Swaption bermudanSwaptionC(underlyingCall2, bermudanExercise);
        Swaption bermudanSwaptionP(underlyingPut2, bermudanExercise);
        bermudanSwaptionC.setPricingEngine(mfSwaptionEngine1);
        bermudanSwaptionP.setPricingEngine(mfSwaptionEngine1);

        pricePay = bermudanSwaptionC.NPV();
        priceRcv = bermudanSwaptionP.NPV();

    } else {
        // calculate european here //
        QuantLib::ext::shared_ptr<BlackSwaptionEngine> blackSwaptionEngine1(new BlackSwaptionEngine(yldCrv, volCube));

        QuantLib::ext::shared_ptr<Exercise> exercise(new EuropeanExercise(startDate));

        //     Rprintf("%d %d %d\n",outputs1.expiries_[i].dayOfMonth(),outputs1.expiries_[i].month(),outputs1.expiries_[i].year());

        Swaption swaptionC(underlyingCall, exercise);
        Swaption swaptionP(underlyingPut, exercise);
        swaptionC.setPricingEngine(blackSwaptionEngine1);
        swaptionP.setPricingEngine(blackSwaptionEngine1);
        pricePay = swaptionC.NPV();
        priceRcv = swaptionP.NPV();
    }
    ////////men at work/////////////////

    return Rcpp::List::create(Rcpp::Named("pay") = pricePay,
                              Rcpp::Named("rcv") = priceRcv,
                              Rcpp::Named("sigma") = vol,
                              Rcpp::Named("atmRate") = rate);
}
