
//  RQuantLib function AffineSwaption
//
//  Copyright (C) 2005 - 2007  Dominick Samperi
//  Copyright (C) 2007 - 2020  Dirk Eddelbuettel
//  Copyright (C) 2016         Terry Leitch
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

#include <rquantlib_internal.h>

// Calibrates underlying swaptions to the input volatility matrix.
void calibrateModel2(const QuantLib::ext::shared_ptr<QuantLib::ShortRateModel>& model,
                     const std::vector<QuantLib::ext::shared_ptr<QuantLib::BlackCalibrationHelper>> &swaptions,
                     QuantLib::Real lambda,
                     Rcpp::NumericVector &swaptionMat,
                     Rcpp::NumericVector &swapLengths,
                     Rcpp::NumericVector &swaptionVols) {

    std::vector<QuantLib::ext::shared_ptr<QuantLib::CalibrationHelper>>
        helpers(swaptions.begin(), swaptions.end());
    QuantLib::Size numRows = swaptionVols.size();
    //QuantLib::Size numCols = swaptionVols.ncol();
    QuantLib::LevenbergMarquardt om;
    model->calibrate(helpers, om,QuantLib:: EndCriteria(400,100,1.0e-8, 1.0e-8, 1.0e-8));

    // Output the implied Black volatilities
    for (QuantLib::Size i=0; i<numRows; i++) {
        QuantLib::Real npv = swaptions[i]->modelValue();
        QuantLib::Volatility implied = swaptions[i]->impliedVolatility(npv, 1e-4,
                                                                     1000, 0.05, 1.50);
        QuantLib::Volatility diff = implied - swaptionVols(i);

        Rprintf((char*) "%dx%d: model %lf, market %lf, diff %lf\n",
                swaptionMat[i], swapLengths[i], implied,
                swaptionVols(i), diff);
    }
}



// [[Rcpp::export]]
Rcpp::List affineWithRebuiltCurveEngine(Rcpp::List rparam,
                                        Rcpp::List legparams,
                                        std::vector<QuantLib::Date> dateVec,
                                        std::vector<double> zeroVec,
                                        Rcpp::NumericVector swaptionMat,
                                        Rcpp::NumericVector swapLengths,
                                        Rcpp::NumericVector swaptionVols) {

    // std::vector<std::string> tsnames = tslist.names();


    QuantLib::Size i;
    //int *swaptionMat=0, *swapLengths=0;
    //double **swaptionVols=0;

    double notional = 10000; // prices in basis points

    QuantLib::Date todaysDate(Rcpp::as<QuantLib::Date>(rparam["tradeDate"]));
    QuantLib::Date settlementDate(Rcpp::as<QuantLib::Date>(rparam["settleDate"]));
    QuantLib::Date startDate(Rcpp::as<QuantLib::Date>(rparam["startDate"]));
    QuantLib::Date maturity(Rcpp::as<QuantLib::Date>(rparam["maturity"]));
    bool payfix = Rcpp::as<bool>(rparam["payFixed"]);
    bool european = Rcpp::as<bool>(rparam["european"]);


    //cout << "TradeDate: " << todaysDate << endl << "Settle: " << settlementDate << endl;

    RQLContext::instance().settleDate = settlementDate;
    QuantLib::Settings::instance().evaluationDate() = todaysDate;

    // initialise from the singleton instance
    QuantLib::Calendar calendar = RQLContext::instance().calendar;
    //Integer fixingDays = RQLContext::instance().fixingDays;

    double strike = Rcpp::as<double>(rparam["strike"]);
    std::string method = Rcpp::as<std::string>(rparam["method"]);

    QuantLib::Handle<QuantLib::YieldTermStructure>
        rhTermStructure(rebuildCurveFromZeroRates(dateVec, zeroVec));

    // Get swaption maturities
    //Rcpp::NumericVector swaptionMat(maturities);
    int numRows = swaptionMat.size();

    // Create dummy swap to get schedules.
    QuantLib::Frequency fixedLegFrequency = getFrequency(Rcpp::as<double>(legparams["fixFreq"]));
    QuantLib::BusinessDayConvention fixedLegConvention = QuantLib::Unadjusted;
    QuantLib::BusinessDayConvention floatingLegConvention = QuantLib::ModifiedFollowing;
    QuantLib::DayCounter swFixedLegDayCounter = getDayCounter(Rcpp::as<double>(legparams["dayCounter"]));
    QuantLib::ext::shared_ptr<QuantLib::IborIndex> swFloatingLegIndex(new QuantLib::Euribor(QuantLib::Period(Rcpp::as<int>(legparams["floatFreq"]),QuantLib::Months),rhTermStructure));


    QuantLib::Rate dummyFixedRate = 0.03;

    QuantLib::Schedule fixedSchedule(startDate,maturity,
                                     QuantLib::Period(fixedLegFrequency),calendar,
                                     fixedLegConvention,fixedLegConvention,
                                     QuantLib::DateGeneration::Forward,false);
    QuantLib::Schedule floatSchedule(startDate,maturity,QuantLib::Period(Rcpp::as<int>(legparams["floatFreq"]),QuantLib::Months),
                                     calendar,
                                     floatingLegConvention,floatingLegConvention,
                                     QuantLib::DateGeneration::Forward,false);

    QuantLib::VanillaSwap::Type type;

    if (payfix) {
        type = QuantLib::VanillaSwap::Payer;}
    else {
        type = QuantLib::VanillaSwap::Receiver;
    }
    QuantLib::ext::shared_ptr<QuantLib::VanillaSwap>
        swap(new QuantLib::VanillaSwap(type, notional,
                                       fixedSchedule, dummyFixedRate, swFixedLegDayCounter,
                                       floatSchedule, swFloatingLegIndex, 0.0,
                                       swFloatingLegIndex->dayCounter()));
    swap->setPricingEngine(QuantLib::ext::shared_ptr<QuantLib::PricingEngine>(new QuantLib::DiscountingSwapEngine(rhTermStructure)));

    // Find the ATM or break-even rate
    QuantLib::Rate fixedATMRate = swap->fairRate();

    QuantLib::Rate fixedRate;
    if(strike < 0) // factor instead of real strike
        fixedRate = fixedATMRate * (-strike);
    else
        fixedRate = strike;

    // The swap underlying the Affine swaption.
    QuantLib::ext::shared_ptr<QuantLib::VanillaSwap>
        mySwap(new QuantLib::VanillaSwap(type, notional,
                                         fixedSchedule, fixedRate,swFixedLegDayCounter,
                                         floatSchedule, swFloatingLegIndex, 0.0,
                                         swFloatingLegIndex->dayCounter()));
    swap->setPricingEngine(QuantLib::ext::shared_ptr<QuantLib::PricingEngine>(new QuantLib::DiscountingSwapEngine(rhTermStructure)));


    // Build swaptions that will be used to calibrate model to
    // the volatility matrix.
    std::vector<QuantLib::Period> swaptionMaturities;
    for(i = 0; i < (QuantLib::Size)numRows; i++)
        swaptionMaturities.push_back(QuantLib::Period(swaptionMat[i], QuantLib::Years));

    // Swaptions used for calibration
    std::vector<QuantLib::ext::shared_ptr<QuantLib::BlackCalibrationHelper> > swaptions;

    // List of times that have to be included in the timegrid
    std::list<QuantLib::Time> times;
    for (i=0; i<(QuantLib::Size)numRows; i++) {
        //QuantLib::ext::shared_ptr<QuantLib::Quote> vol(new QuantLib::SimpleQuote(swaptionVols[i][numCols-i-1]));
        QuantLib::ext::shared_ptr<QuantLib::Quote> vol(new QuantLib::SimpleQuote(swaptionVols(i)));
        swaptions.push_back(QuantLib::ext::shared_ptr<QuantLib::BlackCalibrationHelper>(new QuantLib::SwaptionHelper(swaptionMaturities[i],
                                                                                                             QuantLib::Period(swapLengths[i], QuantLib::Years),
                                                                                                             QuantLib::Handle<QuantLib::Quote>(vol),
                                                                                                             swFloatingLegIndex,
                                                                                                             swFloatingLegIndex->tenor(),
                                                                                                             swFloatingLegIndex->dayCounter(),
                                                                                                             swFloatingLegIndex->dayCounter(),
                                                                                                             rhTermStructure)));
        swaptions.back()->addTimesTo(times);
    }

    // Building time-grid
    QuantLib::TimeGrid grid(times.begin(), times.end(), 30);


    // Get Affine swaption exercise dates, single date if europen, coupon dates if bermudan
    std::vector<QuantLib::Date> affineDates;
    const std::vector<QuantLib::ext::shared_ptr<QuantLib::CashFlow> >& leg = swap->fixedLeg();
    if(european){
        QuantLib::ext::shared_ptr<QuantLib::Coupon> coupon = QuantLib::ext::dynamic_pointer_cast<QuantLib::Coupon>(leg[0]);
        affineDates.push_back(coupon->accrualStartDate());
    } else{
        for (i=0; i<leg.size(); i++) {
            QuantLib::ext::shared_ptr<QuantLib::Coupon> coupon = QuantLib::ext::dynamic_pointer_cast<QuantLib::Coupon>(leg[i]);
            affineDates.push_back(coupon->accrualStartDate());
        }

    }

    QuantLib::ext::shared_ptr<QuantLib::Exercise> affineExercise(new QuantLib::BermudanExercise(affineDates));

    // Price based on method selected.
    if (method.compare("G2Analytic") == 0) {
        QuantLib::ext::shared_ptr<QuantLib::G2> modelG2(new QuantLib::G2(rhTermStructure));
        Rprintf((char*)"G2/Jamshidian (analytic) calibration\n");
        for(i = 0; i < swaptions.size(); i++)
            swaptions[i]->setPricingEngine(QuantLib::ext::shared_ptr<QuantLib::PricingEngine>(new QuantLib::G2SwaptionEngine(modelG2, 6.0, 16)));
        calibrateModel2(modelG2, swaptions, 0.05, swaptionMat, swapLengths, swaptionVols);
        QuantLib::ext::shared_ptr<QuantLib::PricingEngine> engine(new QuantLib::TreeSwaptionEngine(modelG2, 50));
        QuantLib::Swaption affineSwaption(mySwap, affineExercise);
        affineSwaption.setPricingEngine(engine);
        return Rcpp::List::create(Rcpp::Named("a")         = modelG2->params()[0],
                                  Rcpp::Named("sigma")     = modelG2->params()[1],
                                  Rcpp::Named("b")         = modelG2->params()[2],
                                  Rcpp::Named("eta")       = modelG2->params()[3],
                                  Rcpp::Named("rho")       = modelG2->params()[4],
                                  Rcpp::Named("NPV")       = affineSwaption.NPV(),
                                  Rcpp::Named("ATMStrike") = fixedATMRate);
        //Rcpp::Named("params")    = params);

    } else if (method.compare("HWAnalytic") == 0) {
        QuantLib::ext::shared_ptr<QuantLib::HullWhite> modelHW(new QuantLib::HullWhite(rhTermStructure));
        Rprintf((char*)"Hull-White (analytic) calibration\n");
        for (i=0; i<swaptions.size(); i++)
            swaptions[i]->setPricingEngine(QuantLib::ext::shared_ptr<QuantLib::PricingEngine>(new QuantLib::JamshidianSwaptionEngine(modelHW)));
        calibrateModel2(modelHW, swaptions, 0.05, swaptionMat, swapLengths, swaptionVols);
        QuantLib::ext::shared_ptr<QuantLib::PricingEngine> engine(new QuantLib::TreeSwaptionEngine(modelHW, 50));
        QuantLib::Swaption affineSwaption(mySwap, affineExercise);
        affineSwaption.setPricingEngine(engine);
        return Rcpp::List::create(Rcpp::Named("a") = modelHW->params()[0],
                                  Rcpp::Named("sigma") = modelHW->params()[1],
                                  Rcpp::Named("NPV") = affineSwaption.NPV(),
                                  Rcpp::Named("ATMStrike") = fixedATMRate);
        //Rcpp::Named("params") = params);

    } else if (method.compare("HWTree") == 0) {
        QuantLib::ext::shared_ptr<QuantLib::HullWhite> modelHW2(new QuantLib::HullWhite(rhTermStructure));
        Rprintf((char*)"Hull-White (tree) calibration\n");
        for (i=0; i<swaptions.size(); i++)
            swaptions[i]->setPricingEngine(QuantLib::ext::shared_ptr<QuantLib::PricingEngine>(new QuantLib::TreeSwaptionEngine(modelHW2,grid)));

        calibrateModel2(modelHW2, swaptions, 0.05, swaptionMat, swapLengths, swaptionVols);
        QuantLib::ext::shared_ptr<QuantLib::PricingEngine> engine(new QuantLib::TreeSwaptionEngine(modelHW2, 50));
        QuantLib::Swaption affineSwaption(mySwap, affineExercise);
        affineSwaption.setPricingEngine(engine);
        return Rcpp::List::create(Rcpp::Named("a") = modelHW2->params()[0],
                                  Rcpp::Named("sigma") = modelHW2->params()[1],
                                  Rcpp::Named("NPV") = affineSwaption.NPV(),
                                  Rcpp::Named("ATMStrike") = fixedATMRate);
        //Rcpp::Named("params") = params);


    } else if (method.compare("BKTree") == 0) {
        QuantLib::ext::shared_ptr<QuantLib::BlackKarasinski> modelBK(new QuantLib::BlackKarasinski(rhTermStructure));
        Rprintf((char*)"Black-Karasinski (tree) calibration\n");
        for (i=0; i<swaptions.size(); i++)
            swaptions[i]->setPricingEngine(QuantLib::ext::shared_ptr<QuantLib::PricingEngine>(new QuantLib::TreeSwaptionEngine(modelBK,grid)));
        calibrateModel2(modelBK, swaptions, 0.05, swaptionMat, swapLengths, swaptionVols);

        QuantLib::ext::shared_ptr<QuantLib::PricingEngine> engine(new QuantLib::TreeSwaptionEngine(modelBK, 50));
        QuantLib::Swaption affineSwaption(mySwap, affineExercise);
        affineSwaption.setPricingEngine(engine);
        return Rcpp::List::create(Rcpp::Named("a") = modelBK->params()[0],
                                  Rcpp::Named("sigma") = modelBK->params()[1],
                                  Rcpp::Named("price") = affineSwaption.NPV(),
                                  Rcpp::Named("ATMStrike") = fixedATMRate);
        //Rcpp::Named("params") = params);

    } else {
        throw std::range_error("Unknown method in AffineSwaption\n");
    }

}
