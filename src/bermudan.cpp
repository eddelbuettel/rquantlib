// -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- 
//
// RQuantLib function BermudanSwaption
//
// Copyright (C) 2005 - 2007 Dominick Samperi
// Copyright (C) 2007 - 2010 Dirk Eddelbuettel
//
// $Id$
//
// This program is part of the RQuantLib library for R (GNU S).
// It is made available under the terms of the GNU General Public
// License, version 2, or at your option, any later version.
//
// This program is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the GNU General Public License for more
// details.

#include "rquantlib.hpp"

// Calibrates underlying swaptions to the input volatility matrix.
void calibrateModel(const boost::shared_ptr<ShortRateModel>& model,
                    const std::vector<boost::shared_ptr<CalibrationHelper> > &helpers,
                    Real lambda,
                    Rcpp::NumericVector &swaptionMat, 
                    Rcpp::NumericVector &swapLengths, 
                    Rcpp::NumericMatrix &swaptionVols) {

    Size numRows = swaptionVols.nrow();
    Size numCols = swaptionVols.ncol();
    LevenbergMarquardt om;
    model->calibrate(helpers, om, EndCriteria(400,100,1.0e-8, 1.0e-8, 1.0e-8));

    // Output the implied Black volatilities
    for (Size i=0; i<numRows; i++) {
	Real npv = helpers[i]->modelValue();
	Volatility implied = helpers[i]->impliedVolatility(npv, 1e-4,
                                                           1000, 0.05, 0.50);
	Volatility diff = implied - swaptionVols(i, numCols-i-1);

	Rprintf((char*) "%dx%d: model %lf, market %lf, diff %lf\n",
            swaptionMat[i], swapLengths[numCols-i-1], implied, 
            swaptionVols(i, numCols-i-1), diff);
    }	
}	

RcppExport SEXP QL_BermudanSwaption(SEXP params, SEXP tsQuotes, 
                                    SEXP maturities, SEXP tenors, 
                                    SEXP vols) {

    try {

        Rcpp::List rparam(params);
        Rcpp::List tslist(tsQuotes);
        std::vector<std::string> tsnames = tslist.names();

        Size i;
        //int *swaptionMat=0, *swapLengths=0;
        //double **swaptionVols=0;

        double notional = 10000; // prices in basis points

        Date todaysDate(dateFromR(Rcpp::as<int>(rparam["tradeDate"]))); 
        Date settlementDate(dateFromR(Rcpp::as<int>(rparam["settleDate"]))); 
        //cout << "TradeDate: " << todaysDate << endl << "Settle: " << settlementDate << endl;

        RQLContext::instance().settleDate = settlementDate;
        Settings::instance().evaluationDate() = todaysDate;

        double strike = Rcpp::as<double>(rparam["strike"]);
        std::string method = Rcpp::as<std::string>(rparam["method"]);

        std::string firstQuoteName = tsnames[0];  //tslist.getName(0);

        std::string interpWhat, interpHow;
        if(firstQuoteName.compare("flat") != 0) {
            // Get interpolation method (not needed for "flat" case)
            interpWhat = Rcpp::as<std::string>(rparam["interpWhat"]);
            interpHow  = Rcpp::as<std::string>(rparam["interpHow"]);
        }

        // initialise from the singleton instance
        Calendar calendar = RQLContext::instance().calendar;
        //Integer fixingDays = RQLContext::instance().fixingDays;

        // Any DayCounter would be fine.
        // ActualActual::ISDA ensures that 30 years is 30.0
        DayCounter termStructureDayCounter = ActualActual(ActualActual::ISDA);
        double tolerance = 1.0e-15;

        boost::shared_ptr<YieldTermStructure> curve;
        if(firstQuoteName.compare("flat") == 0) {
            // Get flat yield curve
            double rateQuote = tslist[0]; //tslist.getValue(0);
            boost::shared_ptr<Quote> flatRate(new SimpleQuote(rateQuote));
            boost::shared_ptr<FlatForward> ts(new FlatForward(settlementDate,
                                                              Handle<Quote>(flatRate),
                                                              Actual365Fixed()));
            curve = ts;
        }
        else {
            // Get yield curve based on a set of market rates and/or prices.
            std::vector<boost::shared_ptr<RateHelper> > curveInput;
            for(i = 0; i < (Size)tslist.size(); i++) {
                std::string name = tsnames[i]; //tslist.getName(i);
                double val = tslist[i]; //tslist.getValue(i);
                boost::shared_ptr<RateHelper> rh = 
                    ObservableDB::instance().getRateHelper(name, val);
                // edd 2009-11-01 FIXME NULL_RateHelper no longer builds under 0.9.9
                // if(rh == NULL_RateHelper)
                if (rh.get() == NULL)
                    throw std::range_error("Unknown rate in getRateHelper");
                curveInput.push_back(rh);
            }
            boost::shared_ptr<YieldTermStructure> ts =
                getTermStructure(interpWhat, interpHow, 
                                 settlementDate, curveInput,
                                 termStructureDayCounter, tolerance);
            curve = ts;
        }
        //Handle<YieldTermStructure> rhTermStructure;
        //rhTermStructure.linkTo(curve);
        boost::shared_ptr<Quote> flatRate(new SimpleQuote(0.04875825));  // FIXME: hardcoded?
        Handle<YieldTermStructure> 
            rhTermStructure(boost::shared_ptr<FlatForward>(new FlatForward(settlementDate, 
                                                                           Handle<Quote>(flatRate),
                                                                           Actual365Fixed())));

        // Get swaption vol matrix.
        Rcpp::NumericMatrix swaptionVols(vols);
        int dim1 = swaptionVols.nrow(); 
        int dim2 = swaptionVols.ncol(); 
	
        // Get swaption maturities
        Rcpp::NumericVector swaptionMat(maturities);
        int numRows = swaptionMat.size(); 

        // Get swap tenors
        Rcpp::NumericVector swapLengths(tenors);
        int numCols = swapLengths.size(); 

        if (numRows*numCols != dim1*dim2) {
            std::ostringstream oss;
            oss << "Swaption vol matrix size (" << dim1 << " x " << dim2 << ") "
                << "incompatible\nwith size of swaption maturity vector ("
                << numRows << ") and swap tenor vector ("
                << numCols << ")";
            throw std::range_error(oss.str());
        }

        // Create dummy swap to get schedules.
        Frequency fixedLegFrequency = Annual;
        BusinessDayConvention fixedLegConvention = Unadjusted;
        BusinessDayConvention floatingLegConvention = ModifiedFollowing;
        DayCounter fixedLegDayCounter = Thirty360(Thirty360::European);
        Frequency floatingLegFrequency = Semiannual;
        Rate dummyFixedRate = 0.03;
        boost::shared_ptr<IborIndex> indexSixMonths(new Euribor6M(rhTermStructure));
        
        Date startDate = calendar.advance(settlementDate,1,Years, floatingLegConvention);
        Date maturity = calendar.advance(startDate,5,Years, floatingLegConvention);
        Schedule fixedSchedule(startDate,maturity,
                               Period(fixedLegFrequency),calendar,
                               fixedLegConvention,fixedLegConvention,
                               DateGeneration::Forward,false);
        Schedule floatSchedule(startDate,maturity,Period(floatingLegFrequency),
                               calendar,
                               floatingLegConvention,floatingLegConvention,
                               DateGeneration::Forward,false);
        VanillaSwap::Type type = VanillaSwap::Payer;
        boost::shared_ptr<VanillaSwap> 
            swap(new VanillaSwap(type, notional,
                                 fixedSchedule, dummyFixedRate, fixedLegDayCounter,
                                 floatSchedule, indexSixMonths, 0.0,
                                 indexSixMonths->dayCounter()));
        swap->setPricingEngine(boost::shared_ptr<PricingEngine>(new DiscountingSwapEngine(rhTermStructure)));
        
        // Find the ATM or break-even rate
        Rate fixedATMRate = swap->fairRate();

        Rate fixedRate;
        if(strike < 0) // factor instead of real strike
            fixedRate = fixedATMRate * (-strike);
        else
            fixedRate = strike;
        
        // The swap underlying the Bermudan swaption.
        boost::shared_ptr<VanillaSwap> 
            mySwap(new VanillaSwap(type, notional,
                                   fixedSchedule, fixedRate, fixedLegDayCounter,
                                   floatSchedule, indexSixMonths, 0.0,
                                   indexSixMonths->dayCounter()));
        swap->setPricingEngine(boost::shared_ptr<PricingEngine>(new DiscountingSwapEngine(rhTermStructure)));

        // Build swaptions that will be used to calibrate model to
        // the volatility matrix.
        std::vector<Period> swaptionMaturities;
        for(i = 0; i < (Size)numRows; i++)
            swaptionMaturities.push_back(Period(swaptionMat[i], Years));

        // Swaptions used for calibration
        std::vector<boost::shared_ptr<CalibrationHelper> > swaptions;

        // List of times that have to be included in the timegrid
        std::list<Time> times;
        for (i=0; i<(Size)numRows; i++) {
            //boost::shared_ptr<Quote> vol(new SimpleQuote(swaptionVols[i][numCols-i-1]));
            boost::shared_ptr<Quote> vol(new SimpleQuote(swaptionVols(i, numCols-i-1)));
            swaptions.push_back(boost::shared_ptr<CalibrationHelper>(new SwaptionHelper(swaptionMaturities[i],
                                                                                        Period(swapLengths[numCols-i-1], Years),
                                                                                        Handle<Quote>(vol),
                                                                                        indexSixMonths,
                                                                                        indexSixMonths->tenor(),
                                                                                        indexSixMonths->dayCounter(),
                                                                                        indexSixMonths->dayCounter(),
                                                                                        rhTermStructure)));
            swaptions.back()->addTimesTo(times);
        }

        // Building time-grid
        TimeGrid grid(times.begin(), times.end(), 30);

        // Get Bermudan swaption exercise dates.
        std::vector<Date> bermudanDates;
        const std::vector<boost::shared_ptr<CashFlow> >& leg = swap->fixedLeg();
        for (i=0; i<leg.size(); i++) {
            boost::shared_ptr<Coupon> coupon = boost::dynamic_pointer_cast<Coupon>(leg[i]);
            bermudanDates.push_back(coupon->accrualStartDate());
        }
        boost::shared_ptr<Exercise> bermudaExercise(new BermudanExercise(bermudanDates));

        // Price based on method selected.
        if (method.compare("G2Analytic") == 0) {

            boost::shared_ptr<G2> modelG2(new G2(rhTermStructure));
            Rprintf((char*)"G2/Jamshidian (analytic) calibration\n");
            for(i = 0; i < swaptions.size(); i++)
                swaptions[i]->setPricingEngine(boost::shared_ptr<PricingEngine>(new G2SwaptionEngine(modelG2, 6.0, 16)));
            calibrateModel(modelG2, swaptions, 0.05, swaptionMat, swapLengths, swaptionVols); 
            boost::shared_ptr<PricingEngine> engine(new TreeSwaptionEngine(modelG2, 50));
            Swaption bermudanSwaption(mySwap, bermudaExercise); 
            bermudanSwaption.setPricingEngine(engine);
            return Rcpp::List::create(Rcpp::Named("a")         = modelG2->params()[0],
                                      Rcpp::Named("sigma")     = modelG2->params()[1],
                                      Rcpp::Named("b")         = modelG2->params()[2],
                                      Rcpp::Named("eta")       = modelG2->params()[3],
                                      Rcpp::Named("rho")       = modelG2->params()[4],
                                      Rcpp::Named("price")     = bermudanSwaption.NPV(),
                                      Rcpp::Named("ATMStrike") = fixedATMRate,
                                      Rcpp::Named("params")    = params);

        } else if (method.compare("HWAnalytic") == 0) {

            boost::shared_ptr<HullWhite> modelHW(new HullWhite(rhTermStructure));
            Rprintf((char*)"Hull-White (analytic) calibration\n");
            for (i=0; i<swaptions.size(); i++)
                swaptions[i]->setPricingEngine(boost::shared_ptr<PricingEngine>(
                                                                                new JamshidianSwaptionEngine(modelHW)));
            calibrateModel(modelHW, swaptions, 0.05, swaptionMat, swapLengths, swaptionVols);
            boost::shared_ptr<PricingEngine> engine(new TreeSwaptionEngine(modelHW, 50));
            Swaption bermudanSwaption(mySwap, bermudaExercise);
            bermudanSwaption.setPricingEngine(engine);
            return Rcpp::List::create(Rcpp::Named("a") = modelHW->params()[0],
                                      Rcpp::Named("sigma") = modelHW->params()[1],
                                      Rcpp::Named("price") = bermudanSwaption.NPV(),
                                      Rcpp::Named("ATMStrike") = fixedATMRate,
                                      Rcpp::Named("params") = params);

        } else if (method.compare("HWTree") == 0) {
            boost::shared_ptr<HullWhite> modelHW2(new HullWhite(rhTermStructure));
            Rprintf((char*)"Hull-White (tree) calibration\n");
            for (i=0; i<swaptions.size(); i++)
                swaptions[i]->setPricingEngine(boost::shared_ptr<PricingEngine>(new TreeSwaptionEngine(modelHW2,grid)));

            calibrateModel(modelHW2, swaptions, 0.05, swaptionMat, swapLengths, swaptionVols);
            boost::shared_ptr<PricingEngine> engine(new TreeSwaptionEngine(modelHW2, 50));
            Swaption bermudanSwaption(mySwap, bermudaExercise);
            bermudanSwaption.setPricingEngine(engine);
            return Rcpp::List::create(Rcpp::Named("a") = modelHW2->params()[0],
                                      Rcpp::Named("sigma") = modelHW2->params()[1],
                                      Rcpp::Named("price") = bermudanSwaption.NPV(),
                                      Rcpp::Named("ATMStrike") = fixedATMRate,
                                      Rcpp::Named("params") = params);

        } else if (method.compare("BKTree") == 0) {
            boost::shared_ptr<BlackKarasinski> modelBK(new
                                                       BlackKarasinski(rhTermStructure));
            Rprintf((char*)"Black-Karasinski (tree) calibration\n");
            for (i=0; i<swaptions.size(); i++)
                swaptions[i]->setPricingEngine(boost::shared_ptr<PricingEngine>(
                                                                                new TreeSwaptionEngine(modelBK,grid)));
            calibrateModel(modelBK, swaptions, 0.05, swaptionMat, swapLengths, swaptionVols);

            boost::shared_ptr<PricingEngine> engine(new TreeSwaptionEngine(modelBK, 50));
            Swaption bermudanSwaption(mySwap, bermudaExercise);
            bermudanSwaption.setPricingEngine(engine);
            return Rcpp::List::create(Rcpp::Named("a") = modelBK->params()[0],
                                      Rcpp::Named("sigma") = modelBK->params()[1],
                                      Rcpp::Named("price") = bermudanSwaption.NPV(),
                                      Rcpp::Named("ATMStrike") = fixedATMRate,
                                      Rcpp::Named("params") = params);
 
        } else {
            throw std::range_error("Unknown method in BermudanSwaption\n");
        }

    } catch(std::exception &ex) { 
        forward_exception_to_r(ex); 
    } catch(...) { 
        ::Rf_error("c++ exception (unknown reason)"); 
    }

    return R_NilValue;

}

